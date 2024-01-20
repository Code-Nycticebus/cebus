from pathlib import Path


src = Path("./src")

for file in src.rglob("*.h"):
    with open(file.parent / ".README.md", "w") as f:
        f.truncate()


for file in src.rglob("*.h"):
    with open(file.parent / "README.md", "a") as f:
        f.write(f"# [{file.name}]({file.name})\n")
