from sys import argv, exit
import os
import re


def make_script(
    num_processes_list=[1, 2, 4, 8, 16, 20, 32], sizes_list=[128, 256, 512, 1024, 2048, 4096]
):
    with open("mpijob.lsf", "w") as f:
        header = ( 
            "source /polusfs/setenv/setup.SMPI\n"
            + f"#BSUB -n {num_processes_list[-1]} -q normal\n"
            + "#BSUB -W 03:00\n"
            + "#BSUB -o result.out\n"
            + "#BSUB -e result.err\n"
	)
     

        f.write(header)
        for size in sizes_list:
            f.write(f'echo "========= MATRIX: {size} x {size} ========="\n')
            for num_processes in num_processes_list:
                f.write(f'echo "========= PROCESSES: {num_processes} ========="\n')
                f.write(f"OMP_NUM_THREADS=1 mpiexec -n {num_processes} ./main {size}\n")


if __name__ == "__main__":

    num_processes_list = None
    sizes_list = None

    if len(argv) > 3:
        print("Command line format:\n\t[num-processes-list, sizes-list]")
        exit(0)
    elif len(argv) == 3:
        sizes_list = [
            int(elem) for elem in re.findall(r"\w+", re.sub(r"[^ \d]", " ", argv[1]))
        ]
        num_processes_list = [
            int(elem) for elem in re.findall(r"\w+", re.sub(r"[^ \d]", " ", argv[2]))
        ]
        make_script(sizes_list, num_processes_list)
    else:
        make_script()

    os.system(". ./mpi_source.sh")
    os.system("bsub < mpijob.lsf")
