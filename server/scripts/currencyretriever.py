import xmlschema
import requests
xs = xmlschema.XMLSchema('./../resources/nbrfxrates.xsd')

response = requests.get('http://www.bnr.ro/nbrfxrates.xml')

currency = xs.to_dict(response.content)
content = str(currency['Body']['Cube'][0]['@date']) + ',' + str(float(currency['Body']['Cube'][0]['Rate'][10]['$'])) + ',' + str(float(currency['Body']['Cube'][0]['Rate'][28]['$']))
print content