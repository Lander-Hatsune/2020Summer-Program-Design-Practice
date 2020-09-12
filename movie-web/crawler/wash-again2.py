import re

pat = re.compile(r"&#\d{3};")


with open('actors-washed.json') as f:
    text = f.read()

text = pat.sub(string=text, repl='-')

with open('actors-washed-washed.json', 'w') as f:
    f.write(text)


with open('movies-washed.json') as f:
    text = f.read()

text = pat.sub(string=text, repl='-')

with open('movies-washed-washed.json', 'w') as f:
    f.write(text)
    
