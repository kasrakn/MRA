import xml.etree.ElementTree as ET

path = "../outputs/6/chr1-output.xml"
# path = 'test.xml'

tree = ET.parse(path)
root = tree.getroot()

# for child in root.iter('rank'):
#     print(child.text)

for child in root[2][1][0]:
    print(child.tag)

# for len in root.iter("length"):
#     print(len.text)