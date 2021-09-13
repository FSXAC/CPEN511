int main(void) {
    int sum = 0;
    for (int i = 0; i < 30; i++) {
        if (i % 2 == 0) {
            sum += i;
        }
    }
    return sum;
}
