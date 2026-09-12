# Differences Between C and C#

A quick, scannable comparison between the **C** and **C#** programming languages.

## Summary Table

| Feature | C | C# |
| :--- | :--- | :--- |
| **Paradigm** | Procedural (Functional/Structured) | Object-Oriented (OOP) |
| **Platform** | Native (Compiles to machine code) | Managed (Runs on .NET CLR virtual machine) |
| **Memory Management** | Manual (`malloc`, `free`) | Automatic (Garbage Collector) |
| **Pointer Support** | Native and heavily used | Restricted (Allowed only in `unsafe` blocks) |
| **Primary Use Case** | Systems programming, OS, Embedded systems | Enterprise apps, Web, Game dev (Unity) |

## Key Differences

* **Memory Safety:** C gives full control over memory, which can lead to leaks if not managed. C# automates this to prevent common bugs.
* **Speed & Performance:** C is faster and lighter because it compiles directly to hardware. C# carries the overhead of a runtime environment.
* **Syntax & Typing:** C is weakly typed and low-level. C# is strongly typed, modern, and safer to write.
