```powershell run id=sensor
class Sensor {
    hidden [System.Collections.Generic.List[scriptblock]] $Observers = [System.Collections.Generic.List[scriptblock]]::new()

    [scriptblock] Subscribe([scriptblock] $observer) {
        $this.Observers.Add($observer)
        $list = $this.Observers
        return { $list.Remove($observer) | Out-Null }.GetNewClosure()
    }

    [void] Set([double] $value) {
        foreach ($observer in @($this.Observers)) {
            & $observer $value
        }
    }
}

$sensor = [Sensor]::new()
$removeDisplay = $sensor.Subscribe({ param($value) Write-Host ('display: {0:F1}' -f $value) })
$null = $sensor.Subscribe({
    param($value)
    if ($value -gt 30) { Write-Host ('alarm: {0:F1} is above 30' -f $value) }
})
$sensor.Set(21.5)
$sensor.Set(30.5)
& $removeDisplay
$sensor.Set(31.0)
```

```output for=sensor
display: 21.5
display: 30.5
alarm: 30.5 is above 30
alarm: 31.0 is above 30
```

- Verdict: idiom. A list of script blocks is the observer list, and `&` calls each one.
- Output from a method of a class does not go to the pipeline, so the observers write with `Write-Host`.
- `GetNewClosure()` makes the removal script block keep the list and the observer after `Subscribe` returns.
- `Register-ObjectEvent` subscribes to the events of a .NET object. PowerShell puts each event in a queue, and the action runs when the session handles the queue, not inside the raise.
