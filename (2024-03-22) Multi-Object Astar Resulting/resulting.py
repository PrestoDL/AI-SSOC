import matplotlib.pyplot as plt
import matplotlib.animation as animation



file_path = 'resultMap.txt'
file = open(file_path, 'r')

'''
x_range, y_range = map(int, (file.readline()).split())
'''

x_range = 100
y_range = 100
lines = file.readlines()
z_range = int(len(lines) / y_range)
timestamp = [[[int(lines[k * y_range + j].split()[i]) for i in range(x_range)] for j in range(y_range)] for k in range(z_range)]
file.close()


fig, ax = plt.subplots()
def update(i):
    ax.cla()
    ax.matshow(timestamp[i])

ani = animation.FuncAnimation(fig, update, frames = z_range, interval = 10, repeat_delay = 2000)
animation.Animation.save(ani, "resultMap.gif")
plt.show()





'''
file_path = 'result1.txt'
file = open(file_path, 'r')

x_range, y_range = map(int, (file.readline()).split())
lines = file.readlines()
z_range = int(len(lines) / y_range)
timestamp = [[[int(lines[k * y_range + j].split()[i]) for i in range(x_range)] for j in range(y_range)] for k in range(z_range)]
file.close()


fig, ax = plt.subplots()
def update(i):
    timestamp[i][7][7] = -100
    ax.cla()
    ax.matshow(timestamp[i])

ani = animation.FuncAnimation(fig, update, frames = z_range, interval = 200, repeat_delay = 2000)
animation.Animation.save(ani, "test1.gif")
plt.show()
'''


animation.Animation.save(ani, "test3.gif")
plt.show()
'''
