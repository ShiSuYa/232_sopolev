import threading
import random

array = []
total_sum = 0
sum_lock = threading.Lock()

class ThreadData:
    def __init__(self, start, end):
        self.start = start
        self.end = end

def sum_part(data):
    global total_sum
    local_sum = sum(array[data.start:data.end])
    
    with sum_lock:
        total_sum += local_sum

def main():
    global array

    N = int(input("Enter the number of elements (N >= 16): "))
    if N < 16:
        print("Error: N must be at least 16.")
        return

    array = [random.randint(0, 99) for _ in range(N)]

    print("Array elements:")
    print(' '.join(map(str, array)))

    part_size = N // 4
    threads = []
    for i in range(4):
        start = i * part_size
        end = N if i == 3 else (i + 1) * part_size
        data = ThreadData(start, end)
        thread = threading.Thread(target=sum_part, args=(data,))
        threads.append(thread)
        thread.start()

    for thread in threads:
        thread.join()

    print("Total sum =", total_sum)

if __name__ == "__main__":
    main()
