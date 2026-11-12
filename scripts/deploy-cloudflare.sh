#!/bin/sh
# Build the site of tamnd/programming and deploy it to the Worker programming at https://programming.tamnd.com.
#
# Run it from cron on a build server, for example every 5 minutes:
#
#   */5 * * * * $HOME/programming-deploy/deploy-cloudflare.sh >> $HOME/programming-deploy/deploy.log 2>&1; cp $HOME/programming-deploy/content/scripts/deploy-cloudflare.sh $HOME/programming-deploy/ 2>/dev/null
#
# The server needs git, curl, tar, sha256sum, flock, and Node with npx, and about 1 GB of free disk and 2 GB of free memory while it builds. It does not need Go: the script gets the prog release that .github/workflows/ci.yml pins and checks it against SHA256SUMS. The directory ($PROG_DEPLOY, default $HOME/programming-deploy) holds cloudflare.env with CLOUDFLARE_API_TOKEN and CLOUDFLARE_ACCOUNT_ID, mode 600.
#
# The script fetches main, and it gets the test results of the last CI run on main from the release ci-results. It does nothing when main, the prog version and the results are the same as at the last deploy. Use -f to deploy again. It removes the site tree after the deploy, because the disk of the server is small.
set -eu
# cron gives a short PATH. Node and npx are often in /usr/local/bin.
PATH=$PATH:/usr/local/bin:/usr/local/sbin; export PATH

dir=${PROG_DEPLOY:-$HOME/programming-deploy}
repo=https://github.com/tamnd/programming
force=${1:-}

mkdir -p "$dir"
exec 9>"$dir/lock"
flock -n 9 || { echo "another deploy is running"; exit 0; }

set -a
. "$dir/cloudflare.env"
set +a

if [ -d "$dir/content/.git" ]; then
	git -C "$dir/content" fetch -q --depth 1 origin main
	git -C "$dir/content" reset -q --hard FETCH_HEAD
else
	git clone -q --depth 1 "$repo.git" "$dir/content"
fi
head=$(git -C "$dir/content" rev-parse HEAD)
version=$(sed -n 's/^ *PROG_VERSION: *//p' "$dir/content/.github/workflows/ci.yml" | head -n 1 | tr -d "\"' ")
case $version in
v[0-9]*.[0-9]*.[0-9]*) ;;
*) echo "$(date -u +%FT%TZ) no PROG_VERSION in ci.yml"; exit 1 ;;
esac

# The test results are optional: with none, the pages say that the tests did not run.
rm -rf "$dir/results.new"
mkdir -p "$dir/results.new"
if curl -fsSL -o "$dir/results.new/results.tar.gz" "$repo/releases/download/ci-results/results.tar.gz"; then
	results=$(sha256sum "$dir/results.new/results.tar.gz" | cut -c1-16)
else
	results=none
fi

state="$head $version $results"
if [ "$force" != "-f" ] && [ -f "$dir/deployed" ] && [ "$(cat "$dir/deployed")" = "$state" ]; then
	rm -rf "$dir/results.new"
	exit 0
fi
echo "$(date -u +%FT%TZ) deploy programming $head with prog $version, results $results"

case $(uname -m) in
x86_64 | amd64) arch=amd64 ;;
aarch64 | arm64) arch=arm64 ;;
*) echo "no prog release for $(uname -m)"; exit 1 ;;
esac
bin="$dir/bin/prog-$version"
if [ ! -x "$bin" ]; then
	tmp=$(mktemp -d)
	url=https://github.com/tamnd/programming-agent/releases/download/$version
	curl -fsSL -o "$tmp/prog_linux_$arch.tar.gz" "$url/prog_linux_$arch.tar.gz"
	curl -fsSL -o "$tmp/SHA256SUMS" "$url/SHA256SUMS"
	(cd "$tmp" && grep -E "[ /]prog_linux_$arch\.tar\.gz\$" SHA256SUMS | sha256sum -c --quiet -)
	tar -xzf "$tmp/prog_linux_$arch.tar.gz" -C "$tmp"
	mkdir -p "$dir/bin"
	rm -f "$dir"/bin/prog-*
	mv "$tmp/prog_linux_$arch/prog" "$bin"
	rm -rf "$tmp"
fi

cd "$dir/content"
rm -rf build/results "$dir/site"
mkdir -p build/results
if [ "$results" != none ]; then
	tar -xzf "$dir/results.new/results.tar.gz" -C build/results
fi
rm -rf "$dir/results.new"

# The server runs other work, so the build runs at a low CPU and disk priority.
low=""
command -v nice >/dev/null && low="nice -n 15"
command -v ionice >/dev/null && low="$low ionice -c 3"
if ! GOMEMLIMIT=${PROG_BUILD_MEMORY:-2GiB} $low "$bin" site check -out "$dir/site" -minify; then
	rm -rf "$dir/site" "$dir/site.next"
	exit 1
fi
# prog does not print the token. The filter is a second guard, and the exit status of the deploy is kept.
if ! "$bin" site deploy -no-build -out "$dir/site" -commit "$head" >"$dir/deploy.out" 2>&1; then
	grep -v -i token "$dir/deploy.out" || true
	rm -rf "$dir/site" "$dir/deploy.out"
	exit 1
fi
grep -v -i token "$dir/deploy.out" || true
rm -f "$dir/deploy.out"
rm -rf "$dir/site"
echo "$state" >"$dir/deployed"
echo "$(date -u +%FT%TZ) deployed $head"
