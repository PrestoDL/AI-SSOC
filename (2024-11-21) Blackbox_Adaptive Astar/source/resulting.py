import matplotlib.pyplot as plt
import matplotlib.animation as animation



x_range = 100
y_range = 100



def update1(i):
    ax1.cla()
    ax1.matshow(timestamp_1[i])
    
def update2(i):
    ax2.cla()
    ax2.matshow(timestamp_2[i])
    
def update3(i):
    ax3.cla()
    ax3.matshow(timestamp_3[i])
    
def update4(i):
    ax4.cla()
    ax4.matshow(timestamp_4[i])



print("exporting animations...")



with open('output00', 'r') as file:
    lines = file.readlines()
z_range_1 = int(len(lines) / y_range)
timestamp_1 = [[[int(lines[k * y_range + j].split()[i]) for i in range(x_range)] for j in range(y_range)] for k in range(z_range_1)]
file.close()

fig1, ax1 = plt.subplots()
fig1.canvas.manager.set_window_title("Adaptive Astar : Reduction Value 0.0")
ani1 = animation.FuncAnimation(fig1, update1, frames = z_range_1, interval = 10, repeat_delay = 2000)



with open('output04', 'r') as file:
    lines = file.readlines()
z_range_2 = int(len(lines) / y_range)
timestamp_2 = [[[int(lines[k * y_range + j].split()[i]) for i in range(x_range)] for j in range(y_range)] for k in range(z_range_2)]
file.close()

fig2, ax2 = plt.subplots()
fig2.canvas.manager.set_window_title("Adaptive Astar : Reduction Value 0.4")
ani2 = animation.FuncAnimation(fig2, update2, frames = z_range_2, interval = 10, repeat_delay = 2000)



with open('output08', 'r') as file:
    lines = file.readlines()
z_range_3 = int(len(lines) / y_range)
timestamp_3 = [[[int(lines[k * y_range + j].split()[i]) for i in range(x_range)] for j in range(y_range)] for k in range(z_range_3)]
file.close()

fig3, ax3 = plt.subplots()
fig3.canvas.manager.set_window_title("Adaptive Astar : Reduction Value 0.8")
ani3 = animation.FuncAnimation(fig3, update3, frames = z_range_3, interval = 10, repeat_delay = 2000)



with open('output12', 'r') as file:
    lines = file.readlines()
z_range_4 = int(len(lines) / y_range)
timestamp_4 = [[[int(lines[k * y_range + j].split()[i]) for i in range(x_range)] for j in range(y_range)] for k in range(z_range_4)]
file.close()

fig4, ax4 = plt.subplots()
fig4.canvas.manager.set_window_title("Adaptive Astar : Reduction Value 1.2")
ani4 = animation.FuncAnimation(fig4, update4, frames = z_range_4, interval = 10, repeat_delay = 2000)


animation.Animation.save(ani1, "resultMap00.gif")
animation.Animation.save(ani2, "resultMap04.gif")
animation.Animation.save(ani3, "resultMap08.gif")
animation.Animation.save(ani4, "resultMap12.gif")

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




'''
file_path = 'result2.txt'
file = open(file_path, 'r')

x_range, y_range = map(int, (file.readline()).split())
lines = file.readlines()
z_range = int(len(lines) / y_range)
timestamp = [[[int(lines[k * y_range + j].split()[i]) for i in range(x_range)] for j in range(y_range)] for k in range(z_range)]
file.close()


fig, ax = plt.subplots()
def update(i):
    timestamp[i][12][12] = -100
    ax.cla()
    ax.matshow(timestamp[i])

ani = animation.FuncAnimation(fig, update, frames = z_range, interval = 100, repeat_delay = 2000)
animation.Animation.save(ani, "test3.gif")
plt.show()
'''




'''
file_path = 'result3.txt'
file = open(file_path, 'r')

x_range, y_range = map(int, (file.readline()).split())
lines = file.readlines()
z_range = int(len(lines) / y_range)
timestamp = [[[int(lines[k * y_range + j].split()[i]) for i in range(x_range)] for j in range(y_range)] for k in range(z_range)]
file.close()


fig, ax = plt.subplots()
def update(i):
    timestamp[i][9][5] = -100
    ax.cla()
    ax.matshow(timestamp[i])

ani = animation.FuncAnimation(fig, update, frames = z_range, interval = 100, repeat_delay = 2000)
animation.Animation.save(ani, "test3.gif")
plt.show()
'''
