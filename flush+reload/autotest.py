#!/usr/bin/python3

import subprocess
import threading
import time

TEST_STRING = 'abcdefghijklmnopqrstuvwxyz0123456789'

# Calculates the normalized Levenshtein distance of 2 strings
# @Fnguyen from https://datascience.stackexchange.com/questions/60019/damerau-levenshtein-edit-distance-in-python
def levenshtein(s1, s2):
    l1 = len(s1)
    l2 = len(s2)
    matrix = [list(range(l1 + 1))] * (l2 + 1)
    for zz in list(range(l2 + 1)):
      matrix[zz] = list(range(zz,zz + l1 + 1))
    for zz in list(range(0,l2)):
      for sz in list(range(0,l1)):
        if s1[sz] == s2[zz]:
          matrix[zz+1][sz+1] = min(matrix[zz+1][sz] + 1, matrix[zz][sz+1] + 1, matrix[zz][sz])
        else:
          matrix[zz+1][sz+1] = min(matrix[zz+1][sz] + 1, matrix[zz][sz+1] + 1, matrix[zz][sz] + 1)
    distance = float(matrix[l2][l1])
    result = 1.0-distance/max(l1,l2)

    return result

def run_receiver(threshold):
    try:
        result = subprocess.check_output(['./receiver', str(threshold)], input=b'\n', timeout=1).decode('utf-8').splitlines()[-1]
    except subprocess.TimeoutExpired:
        print('0.0,N/A')
        return

    print(f'{levenshtein(TEST_STRING, result)},{result}')

def run_sender():
    subprocess.check_output(['./sender'], input=f'{TEST_STRING}\n'.encode('ascii'))

class AutoTest:
    def compile(verbose=True):
        """
        Simply runs the 'make' command to build all the binaries
        """

        if verbose:
            print("Compiling all files.")
        
        subprocess.run(['make', 'clean'])
        result = subprocess.run(['make', f'CEXTRAS=-D RUN_ONCE'], stdout=subprocess.PIPE)
        result_rc = result.returncode
        result_out = result.stdout.decode('utf-8')

        if verbose:
            print(result_out)

        if result_rc:
            print(f"Compilation failed, return code {result_rc}")
        elif verbose:
            print("Compilation success.")

    def runTest(threshold):
        """
        Runs the sender and receiver program automatically and measure the accuracy
        """

        rt = threading.Thread(target=run_receiver, args=(threshold, ))
        st = threading.Thread(target=run_sender)

        rt.start()
        st.start()

        rt.join()
        st.join()

    def sweepThreshold(start, end, step, sleep=0.5):
        """ output as a csv format """

        AutoTest.compile()

        # csv format header
        print("threshold,distance,string")

        for t in range(start, end, step):
            print(f'{t},', end='')
            AutoTest.runTest(t)
            time.sleep(sleep)

if __name__ == '__main__':
    AutoTest.sweepThreshold(50, 280, 10, slee=0.5)
        