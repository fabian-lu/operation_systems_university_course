public class Counter {

  private int in = 0;

  public synchronized void increment() {
    int next_free_slot = this.in;
    next_free_slot++;
    in = next_free_slot;
  }

  public int getIn() {
    return this.in;
  }

}
