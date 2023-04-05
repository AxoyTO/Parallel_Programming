import re
import pandas as pd


def parse_data(files=["1/1_unoptimized.out", "1/1_optimized.out"]):
    threads_list = []
    times_list = []

    for filename in files:
        tmp_threads_list = []
        tmp_times_list = []

        with open(filename, "r") as file:
            for line in file:
                if line.startswith("Time"):
                    tmp_times_list.append(float(re.search('(\d+.\d+).', line).group(0)))
                    tmp_threads_list.append(int(line.split(" ")[-2]))
                    
            times_list.append(tmp_times_list)
            threads_list.append(tmp_threads_list)
        
    print(times_list)
    print(threads_list)

    data = []

    data.append(
        pd.DataFrame(
            {
                'MatrixSize': 10000,
                'Threads': threads_list[0],
                'Unoptimized': times_list[0],
                'Optimized': times_list[1]
            }))
    
    return data



def write_data_to_excel(data):
    with pd.ExcelWriter('data.xlsx') as writer:
        for i in range(len(data)):
            data[i].to_excel(writer, sheet_name="hi", index=False)


if __name__ == '__main__':
    data = parse_data()
    write_data_to_excel(data)