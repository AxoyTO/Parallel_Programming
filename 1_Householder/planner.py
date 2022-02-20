from sys import argv, exit
import os
import re


def make_script(num_threads_list=[1, 2, 4, 8, 16, 32, 64, 128],
                sizes_list=[512, 1024]
                ):

    with open('ompjob.lsf', 'w') as f:
        header = 'source /polusfs/setenv/setup.SMPI\n' + \
            'export LC_CTYPE=en_US.UTF-8\n' + \
            'export LC_ALL = en_US.UTF-8\n' + \
            '#BSUB -n 20\n' + \
            '#BSUB -W 00:30\n' + \
            '#BSUB -o result.out\n' + \
            '#BSUB -e result.err\n'

        f.write(header)
        for size in sizes_list:
            for num_threads in num_threads_list:
                f.write(
                    f'OMP_NUM_THREADS={num_threads} mpiexec ./main {size} {size} {num_threads}\n')


if __name__ == '__main__':

    num_threads_list = None
    sizes_list = None

    if len(argv) > 3:
        print(
            'Command line format:\n\t[num-threads-list, sizes-list]')
        exit(0)
    elif len(argv) == 3:
        sizes_list = [int(elem) for elem in re.findall(
            r'\w+', re.sub(r'[^ \d]', ' ', argv[1]))]
        num_threads_list = [int(elem) for elem in re.findall(
            r'\w+', re.sub(r'[^ \d]', ' ', argv[2]))]
        make_script(num_threads_list, sizes_list)
    else:
        make_script()

    os.system('g++ main.cpp -o main -fopenmp -std=c++11')
    os.system('bsub < ompjob.lsf')
