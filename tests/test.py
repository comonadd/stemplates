from stemplates import render_template


def test_basic():
    text = """\
Hello, {%username%}"""
    res = render_template(text, username="John")
    assert res == "Hello, John"


def test_cond():
    text = """\
Hello, {% username.capitalize() if capitalize else username %}"""
    res = render_template(text, username="john", capitalize=True)
    assert res == "Hello, John"


def test_loops():
    text = """\
Current users: {% ' '.join(map(lambda user: user.lower(), users)) %}"""
    res = render_template(text, users=["john", "martin", "ben", "alexander"])
    assert res == "Current users: john martin ben alexander"


def test_escape():
    text = """\
Hello, \{%username%}"""
    res = render_template(text, username="John")
    assert res == "Hello, {%username%}"
