#!/env/python
from collections import defaultdict

file1_dict =  defaultdict(list)
with open("g1g2_dumpIOV_log_ReReco.txt_302322.txt") as f1:
    content1 = f1.readlines()
content1 = [x.strip() for x in content1] 
for entry1 in content1:
    tokens1 = entry1.split(" ")
    for token1 in tokens1[1:]:
        file1_dict[tokens1[0]].append(float(token1))

file2_dict =  defaultdict(list)

with open("g1g2_dumpIOV_log_Prompt.txt_303014.txt") as f2:
    content2 = f2.readlines()
content2 = [x.strip() for x in content2] 
for entry2 in content2:
    tokens2 = entry2.split(" ")
    for token2 in tokens2[1:]:
        file2_dict[tokens2[0]].append(float(token2))

print "map1: ",len(file1_dict)," map2: ",len(file2_dict)

# extract the list of detIds
listOfDetIds=[]
for key1,value1 in file1_dict.iteritems():
    listOfDetIds.append(key1)
    
for key2,value2 in file2_dict.iteritems():
    if key2 not in listOfDetIds:
        print "Houston we have a problem!"

fout1=open('./avg_ratio_Run2017E.txt', 'w+')
fout2=open('./max_deviation_ratio1_Run2017E.txt','w+')
fout3=open('./max_deviation_ratio2_Run2017E.txt','w+')

for d in listOfDetIds:
    #print d,max([x/y for x,y in zip(file1_dict[d],file2_dict[d])])
    #fout.write(d+" "+str(max([x/y for x,y in zip(file1_dict[d],file2_dict[d])]))+"\n")
    ratios = [x/y for x,y in zip(file1_dict[d],file2_dict[d])]
    deltas = [abs(y-x) for x,y in zip(file1_dict[d],file2_dict[d])]

    i_max_delta=-1
    max_delta=-1.
    for i,delta in enumerate(deltas):
        if delta>max_delta:     
            i_max_delta=i

    cached_ratio=-999.
    for ratio in ratios:
        if (cached_ratio>=0):
            if (abs(ratio) > abs(cached_ratio)): 
                cached_ratio = ratio
        else:
            cached_ratio = ratio
        
    #print d, sum(ratios)/len(ratios)
    fout1.write(d+" "+str(sum(ratios)/len(ratios))+"\n")
    fout2.write(d+" "+str(ratios[i_max_delta])+"\n")
    fout3.write(d+" "+str(cached_ratio)+"\n")

fout1.close()
fout2.close()
