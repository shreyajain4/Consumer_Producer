# Consumer_Producer
Simultaneous consuming and producing in lockfree queue

HIGH LEVEL DESIGN 

Elements are stored internally using contiguous blocks instead of linked lists for better performance. The queue is made up of a collection of sub-queues, one for each producer. When a consumer wants to dequeue an element, it checks all the sub-queues until it finds one that's not empty. All of this is largely transparent to the user of the queue, however -- it mostly just worksTM.

One particular consequence of this design, however, (which seems to be non-intuitive) is that if two producers enqueue at the same time, there is no defined ordering between the elements when they're later dequeued. Normally this is fine, because even with a fully linearizable queue there'd be a race between the producer threads and so you couldn't rely on the ordering anyway. However, if for some reason you do extra explicit synchronization between the two producer threads yourself, thus defining a total order between enqueue operations, you might expect that the elements would come out in the same total order, which is a guarantee my queue does not offer. At that point, though, there semantically aren't really two separate producers, but rather one that happens to be spread across multiple threads. In this case, you can still establish a total ordering with my queue by creating a single producer token, and using that from both threads to enqueue (taking care to synchronize access to the token, of course, but there was already extra synchronization involved anyway).
