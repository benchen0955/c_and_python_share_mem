from flask import Flask, render_template, request, redirect, url_for
import sysv_ipc
import sys
import time

app = Flask(__name__)

# Create shared memory object
while(True):
	try:
		mem_to_stb = sysv_ipc.SharedMemory(10001)
		mem_from_stb = sysv_ipc.SharedMemory(10000)
		print 'Share memory init done.'
		break;
	except:
		print 'Waiting for share memory ready...'
		time.sleep(1)

@app.route('/set_data/<str>')
def set_data(str):
	mem_to_stb.write(str + '\0')
	return 'Done'

@app.route('/get_data')
def get_data():
	# Read value from shared memory
	memory_value = mem_from_stb.read()

	# Find the 'end' of the string and strip
	i = memory_value.find('\0')
	if i > 0:
		str = memory_value[:i]
	else:
		str = ''

	return str

@app.route('/')
def index():
	return "Hello!!"	

if __name__ == "__main__":
	port = int(sys.argv[1])
	app.run(host = '0.0.0.0', port = port, debug = False)