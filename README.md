An investigation into Networking and Occam-Pi for Raspberry-Pi processors.

Search engines, social networks and scientific measurements are few examples of
nowadays fields heavily relying on computational power. Parallel programming has been
developed to make the most out of multi-core architectures. Yet, there are always underlying
communications at some point between processes, applications, machines, clusters and
so on. This honours project focussed on networking communications for parallel programming
language occam. Robust and inherently parallel, it has been amongst the firsts to approach
the parallel programming world 30 years ago.
Evidently, occam has been provided a network architecture since then: in 2006, called
pOny. Nevertheless, in the last decade newer ones have emerged; being the starting point of
this honours project charged of finding out a better network architecture and implementing
it. It implicitly launched a “Back to occam” conquest where the ultimate aim would be to
integrate this new architecture into occam. This was where net2 came into consideration,
both designed and implemented by Doctor Kevin Chalmers, it demonstrated its efficiency
and became number one contender for occam.
However, occam is not of easy access: it is compiled with KRoC then interpreted by the
Transterpreter. Therefore, the candidate network architecture will have to go through these
two layers. This projects intended to kick it off by developing an implementation in C language
as both layers are so; its potential integration would then be much easier.
As a final output of this honours project, a functional C implementation has effectively
been developed and experimented over several Raspberry-Pis using a distributed application.