import matplotlib.pyplot as plt

# Data from the program output
threads = [1, 2, 3, 4, 5, 6, 7, 8]
speedups = [1.00564, 1.91536, 2.75457, 3.68615, 3.83974, 4.25977, 4.43579, 4.64856]

# Plotting the data
plt.plot(threads, speedups, marker="o", linestyle="-", color="b")

# Adding labels and title
plt.title("Speedup vs Number of Threads")
plt.xlabel("Number of Threads")
plt.ylabel("Speedup (Linear / Parallel)")
plt.grid(True)
plt.savefig("speedup.png")

# Show the plot
plt.show()
