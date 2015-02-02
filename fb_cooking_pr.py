def nextmax(s):
	i=0
	p=[int(x) for x in s]
	n=len(s)
	max_=-1
	max_index=n-1
	for j in xrange(n-1,i,-1):
		if p[j]>max_ and p[j]!=0:
			max_index=j
			max_=p[j]
	if max_>p[0]:
		p[max_index]=p[0]
		p[0]=max_
		return p
	else:
		for i in xrange(1,n-1):
			max_=-1
			max_index=n-1
			for j in xrange(n-1,i,-1):
				if p[j]>max_:
					max_index=j
					max_=p[j]
			if max_>p[i]:
				p[max_index]=p[i]
				p[i]=max_
				return p
	return p

def nextmin(s):
	i=0
	p=[int(x) for x in s]
	n=len(s)
	min_=float('inf')
	min_index=n-1
	for j in xrange(n-1,i,-1):
		if p[j]<min_ and p[j]!=0:
			min_index=j
			min_=p[j]
	if min_<p[0]:
		p[min_index]=p[0]
		p[0]=min_
		return p
	else:
		for i in xrange(1,n-1):
			min_=float('inf')
			min_index=n-1
			for j in xrange(n-1,i,-1):
				if p[j]<min_:
					min_index=j
					min_=p[j]
			if min_<p[i]:
				p[min_index]=p[i]
				p[i]=min_
				return p
	return p
import sys
f=open("input.txt",'r')
fout=open("output.txt",'w')
sys.stdin=f
sys.stdout=fout
t=int(raw_input())
for test in xrange(t):
    s=str(raw_input()).strip()
    min_=''.join(str(i) for i in nextmin(s))
    max_=''.join(str(i) for i in nextmax(s))
    print "Case #"+str(test+1)+": "+min_+" "+max_
    
f.close()
fout.close()