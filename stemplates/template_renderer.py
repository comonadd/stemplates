from renderer import render


def render_template(text: str, **params):
    return render(text, **params)


def render_file(filename: str, **params):
    with open(filename, "r") as f:
        text = f.read()
        return render_template(text)


def render_file_to(filename: str, out_file: str, **params):
    with open(filename, "r") as f:
        text = f.read()
        rendered = render_template(text)
    with open(out_file, "w") as fw:
        fw.write(rendered)
