from setuptools import setup, find_packages

with open("README.md") as f:
    long_description = f.read()

setup(
    name="stemplates",
    version="1.0.0",
    author="Dmitry Guzeev",
    author_email="dmitri.guzeev@gmail.com",
    url="https://github.com/comonadd/stemplates",
    description="Simple file template renderer",
    long_description=long_description,
    long_description_content_type="text/markdown",
    license="MIT",
    packages=find_packages(exclude=("tests",)),
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
    keywords="templates render template file string",
    zip_safe=False,
)
