import xmlschema
#import requests
from pprint import pprint
xs = xmlschema.XMLSchema('./nbrfxrates.xsd')

#response = requests.get('http://www.bnr.ro/nbrfxrates.xml')
#fh = open('test.xml','w')
#fh.write(response.content)
#pprint (response.content)

currency = xs.to_dict('./test.xml')
content = str(currency['Body']['Cube'][0]['@date']) + ',' + str(float(currency['Body']['Cube'][0]['Rate'][10]['$'])) + ',' + str(float(currency['Body']['Cube'][0]['Rate'][28]['$']))
pprint(content)