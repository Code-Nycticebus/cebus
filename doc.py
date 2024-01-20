from pathlib import Path


cwd = Path.cwd()
src = Path(cwd, "src")

doc_files: list[Path] = []

for file in src.rglob("*.h"):
    docs = cwd / "docs" / file.relative_to(src).parent
    docs.mkdir(parents=True, exist_ok=True)
    with open(docs / "README.md", "w") as f:
        f.truncate()

for file in src.rglob("*.h"):
    docs = cwd / "docs" / file.relative_to(src).parent
    with open(docs / "README.md", "a") as f:
        f.write(f"# [{file.name}]({file.relative_to(cwd)})\n")
