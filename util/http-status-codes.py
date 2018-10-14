import csv
import os
import re
import requests

csv_path = '../references/http-status-codes.csv'
header_path = '../include/hypp/status.hpp'
status_codes = []

def slugify(description):
	table = {
		ord('-'): '',
		ord('('): '',
		ord(')'): '',
		ord(' '): '_',
	}
	return description.translate(table)

def get_csv_file():
	r = requests.get('https://www.iana.org/assignments/http-status-codes/http-status-codes-1.csv')
	if r.status_code == 200:
		with open(csv_path, 'wb') as file:
			file.write(r.content)

def parse_csv_file():
	with open(csv_path, 'r', encoding='utf-8', newline='') as file:
		reader = csv.reader(file, delimiter=',', quotechar='"')
		for row in reader:
			if row[0] == 'Value':
				continue # Ignore header
			code = {
				'value': row[0],
				'description': row[1],
				'slug': slugify(row[1]),
				'reference': row[2],
			}
			code['line'] = 'k{0}_{1} = {0},'.format(code['value'], code['slug'])
			code['comment'] = code['reference'].replace('][', '] [')
			status_codes.append(code)

def generate_cpp_code():
	max_width = 0
	for code in status_codes:
		width = len(code['line'])
		if width > max_width:
			max_width = width

	lines = {'codes': [], 'phrases': []}
	for code in status_codes:
		if code['slug'] not in ['Unassigned', 'Unused']: # Ignore unassigned ranges and unused codes
			lines['codes'].append('{}  // {}'.format(code['line'].ljust(max_width), code['comment']))
			lines['phrases'].append('case {}: return "{}";'.format(code['value'], code['description']))

	return lines

def sub_between(source, id, lines):
	pattern = r'( *)(// @{0}-begin)([\r\n]+).*\1(// @{0}-end)'.format(id)
	pattern = re.compile(pattern, flags=re.DOTALL)
	m = pattern.search(source)
	if m:
		code = m.group(1) + '{}{}'.format(m.group(3), m.group(1)).join(lines) + m.group(3)
		code = '{1}{2}{3}{0}{1}{4}'.format(code, m.group(1), m.group(2), m.group(3), m.group(4))
		return pattern.sub(code, source)
	return source

def write_to_header(lines):
	with open(header_path, 'r', encoding='utf-8') as file:
		source = file.read()
	with open(header_path, 'w', encoding='utf-8') as file:
		source = sub_between(source, 'http-status-codes', lines['codes'])
		source = sub_between(source, 'http-status-phrases', lines['phrases'])
		file.write(source)


if not os.path.exists(csv_path) or not os.path.getsize(csv_path):
	get_csv_file()
parse_csv_file()

write_to_header(generate_cpp_code())
