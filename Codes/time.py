from bs4 import BeautifulSoup
import urllib.request

#url = 'https://www.google.com/search?q=san+jose+time'

req = urllib.request.Request(url, headers={'User-Agent': 'Mozilla/5.0'})
html = urllib.request.urlopen(req).read()
  
soup = BeautifulSoup(html)
badges = soup.body.find('div', attrs={'class': '_rkc _Peb'})
print(badges.getText())

