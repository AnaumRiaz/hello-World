from bs4 import BeautifulSoup


#html_snippet = """<div>
#<div class="vk_bk sol-tmp" style="float:left;margin-top:-3px;font-size:64px"><span id="wob_tm" class="wob_t" style="display:inline">18</span><span id="wob_ttm" class="wob_t" style="display:none"> ... </span></div>"""

#soup = BeautifulSoup(html_snippet)
#temp = soup.find("span", id='wob_tm')

#print(temp.string)

import urllib.request
url = 'https://www.google.com/search?q=san+jose+temperature'
req = urllib.request.Request(url, headers={'User-Agent': 'Mozilla/5.0'})
html = urllib.request.urlopen(req).read()
  
soup = BeautifulSoup(html)


badges = soup.body.find('span', attrs={'class': 'wob_t'})
print(badges.getText())
