This area covers how programs store and exchange data. It holds databases and SQL, data formats, regular expressions, text encodings, files and caches.

## What it holds

- **Files.** How a program reads, writes and manages files. The file system as the program sees it, not as the kernel builds it.
- **Paths and directories.** Paths as values and directories as trees.
- **Text encodings.** How text becomes bytes and bytes become text, and the binary-to-text encodings.
- **Text processing.** Line-oriented work on text files: count, sort, filter, compare and rewrite. The other 5 (tokenize, parse an expression, postfix, balanced brackets, recursive descent) are in language-implementation and syntax-and-semantics.
- **Regular expressions in programs.** The cross-language regex tasks and the regex format page. The pattern language is in the regex tree.
- **Data formats.** The text formats that programs use to exchange data: JSON, XML, CSV and Markdown, with their schemas and query languages.
- **Configuration formats.** The formats of configuration files and how to read them.
- **Binary data formats.** Compact binary encodings, schema-based serialization and the reading of binary file formats.
- **Archives and compression.** Compressed streams and archive files from a program. The compression algorithms are on algorithm.tamnd.com.
- **Relational modelling.** How to model data as relations and design a schema. The SQL to create it is in the SQL tree.
- **SQL from programs.** How a program talks to a SQL database: drivers, connections, parameters, migrations and tests. The SQL language itself is in the SQL tree.
- **Transactions in applications.** How application code uses transactions and handles concurrent writers. The SQL statements and isolation levels are in the SQL tree.
- **ORMs.** Object-relational mappers as libraries and the problems of their use. The mapping patterns are in software-design.
- **NoSQL stores.** The data models of non-relational stores and how programs use them. Distribution and replication are in distributed-and-cloud.
- **Caching.** Caches in application code and shared caches between processes. HTTP caching is in web-development.

## What it does not hold

- Database internals, such as B-trees and query planning as algorithms. They are on [the algorithm site](https://algorithm.tamnd.com/).
- Serialization as a language feature. It is in [Data and values](/data-and-values/). This area holds the formats.
- File descriptors and the OS I/O model. They are in [Systems programming](/systems-programming/).
- Big data pipelines. They are in [Data science and ML programming](/data-science-and-ml-programming/).
