import threading
class Timer(threading.Thread):
    def __init__(self, func, frequency):
        super(Timer, self).__init__()
        self.func = func
        self.frequency=frequency
        self.stoppedEvent=threading.Event()

    def run(self):
        while(not self.stoppedEvent.wait(1/self.frequency)):
            self.func()

    def stop(self):
        self.stoppedEvent.set()

    def stopped(self):
        return self.stoppedEvent.isSet()
