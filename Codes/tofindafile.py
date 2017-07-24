
from pathlib import Path

#add the path of the file you wish to know exists or not
my_file = Path("imgs/goldengate.jpeg")

if my_file.is_file():
    print("exists")
else:
    print("no exist")  

