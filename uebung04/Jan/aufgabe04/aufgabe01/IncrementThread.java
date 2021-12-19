public class IncrementThread extends Thread {

    private final int countMax;
    private final Counter counter;
    private final int threadNumber;

    public IncrementThread(int countMax, Counter counter, int threadNumber) {
        this.countMax = countMax;
        this.counter = counter;
        this.threadNumber = threadNumber;
    }

    @Override
    public void run() {
        super.run();

        for (int i = 0; i < countMax; i++) {
            counter.increment();
        }

        System.out.printf("EXIT THREAD %d%n", threadNumber);
    }
}
