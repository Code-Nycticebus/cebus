from pathlib import Path, PurePosixPath
from collections import defaultdict
import subprocess


remote = (
    subprocess.check_output(["git", "config", "--get", "remote.origin.url"])
    .decode()
    .strip()[:-4]
)
cwd = Path.cwd()
src_dir = Path(cwd, "src")
doc_dir = cwd / "docs"


docs_dict: dict[Path, list[Path]] = defaultdict(list)


def write_src_to_doc(doc: Path, src: Path):
    with open(doc, "a") as f:
        f.write(
            f"## [{file.name}]({remote}/blob/main/{PurePosixPath(file.relative_to(cwd))})\n"
        )
        write_mode: bool = False
        with open(src, "r") as d:
            for line in d.readlines():
                # If it encounters the comment terminator it makes 'doc_count' even
                if "*/" in line:
                    write_mode = False

                # If 'doc_count' is odd it's in write mode
                if write_mode:
                    f.write(line)

                # If it encounters the 'DOCUMENTATION' symbol it makes 'doc_count' even
                if "/* DOCUMENTATION" in line:
                    write_mode = True


for file in src_dir.rglob("*.h"):
    doc = doc_dir / file.relative_to(src_dir).parent
    docs_dict[doc].append(file)

readme_file = doc_dir / "README.md"
with open(readme_file, "w") as f:
    f.truncate()

for docs, files in docs_dict.items():
    readme_file.write_text(f"# {docs.name.title()}\n")
    for file in files:
        write_src_to_doc(readme_file, file)
