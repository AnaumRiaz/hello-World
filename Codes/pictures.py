from bs4 import BeautifulSoup
import urllib.request


pla = 'golden gate bridge'
pla = pla.replace(" ", "+")

url = 'https://www.google.com/search?q='+ pla +'+image'
req = urllib.request.Request(url, headers={'User-Agent': 'Mozilla/5.0'})
html = urllib.request.urlopen(req).read()
  
soup = BeautifulSoup(html)
l = soup.findAll('img')[0].get('src')
#above line is finding the src of the first image imbedded in the html of google images
print(l)

#opens an file to copy the image with wb as write back. and name and location of image to save
imagefile = open("imgs/goldengate.jpeg", 'wb')
imagefile.write(urllib.request.urlopen(l).read())
imagefile.close()

