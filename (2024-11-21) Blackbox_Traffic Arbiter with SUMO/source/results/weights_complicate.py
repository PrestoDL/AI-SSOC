import xml.etree.ElementTree as ET

# Path to your SUMO network file
network_file = "ql2.complicate.net.xml"

# Load the network
tree = ET.parse(network_file)
root = tree.getroot()

# Create weights file
weights_file = "weights_complicate.xml"
with open(weights_file, 'w') as f:
    f.write('<edgedata>\n')
    f.write('    <interval begin="0" end="3600">\n')
    for edge in root.findall('edge'):
        edge_id = edge.get('id')
        weight = 1.0  # Example static weight
        f.write(f'        <edge id="{edge_id}" value="{weight}"/>\n')
    f.write('    </interval>\n')
    f.write('</edgedata>\n')

print(f"Weights file '{weights_file}' generated.")