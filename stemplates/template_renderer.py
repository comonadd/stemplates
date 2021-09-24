def render_template(text: str, **params):
    pos = 0

    def get_curr():
        nonlocal pos, text
        if pos < len(text):
            return text[pos]
        return None

    def peek_next():
        nonlocal pos, text
        npos = pos + 1
        if npos < len(text):
            return text[npos]
        return None

    def next_char():
        nonlocal pos
        ch = get_curr()
        pos += 1
        return ch

    res = ""

    while pos < len(text):
        if get_curr() == "{":
            if peek_next() == "%":
                next_char()
                next_char()
                expr = ""
                while not (get_curr() == "%" and peek_next() == "}"):
                    expr += next_char()
                next_char()
                next_char()
                sub = eval(expr, {}, params)
                res += sub
                continue
        res += next_char()

    return res


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
