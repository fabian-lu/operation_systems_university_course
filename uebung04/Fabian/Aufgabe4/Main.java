public class Main extends Thread {

  private int countMax;
  private Counter counter;

  public Main(Counter counter, int countMax) {
    this.counter = counter;
    this.countMax = countMax;
  }

  public static void main(String[] args) {
    int countMax = Integer.parseInt(args[0]);

    Counter counter = new Counter();

    Main firstThread = new Main(counter, countMax);
    Main secondThread = new Main(counter, countMax);
    firstThread.start();
    secondThread.start();

    try {
      firstThread.join();
      secondThread.join();
		} catch (InterruptedException exception) {
      System.out.println("Something went wrong!");
    }

    System.out.println("Result: " + counter.getIn());
  }

  public void run() {
    int i = 0;
    while(i < this.countMax) {
      this.counter.increment();
      i++;
    }
  }
}
