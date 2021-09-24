# stemplates

Render files with embedded python code. Executes `exec` and substitutes
expression with the result.

## Usage

```python
from stemplates import render_template
# Simple substitution
text = "Hello, {% username %}"
rendered = render_template(text, username="John")
# Conditionals
text = "Hello, {% f'{name} {lastname}' if full_name else name %}"
rendered = render_template(text, name="John", lastname="Green", full_name=False)
# Render files
render_file_to("some.template", "out.txt", param1="Param", param2="...")
```

## Install

```bash
pip install stemplates
```
