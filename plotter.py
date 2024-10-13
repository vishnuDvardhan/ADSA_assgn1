import numpy as np
import matplotlib.pyplot as plt


def read_data(filename):
    with open(filename, 'r') as f:
        lines = f.readlines()

    l=[[],[],[],[]]
    k = -1
    i = 0
    while i < len(lines):
        line = lines[i].strip()
        if line in ['all rotations for insertions','all rotations needed for deletions','all heights before deletions','all heights after deletions']:
            k=k+1
            i=i+1
        l[k].append(int(lines[i][:-1]))
        i=i+1
    return np.array(l)


   



filename = '/home/vishnu/assignments/rb_tree_output.txt'


rotations_insertion,rotations_deletion, heights_before_deletion, heights_after_deletion  = read_data(filename)

rotations_insertion = rotations_insertion.reshape(4, -1)
heights_before_deletion = heights_before_deletion.reshape(4, -1)
heights_after_deletion = heights_after_deletion.reshape(4, -1)
rotations_deletion = rotations_deletion.reshape(4, -1)

avg_rotations_insertion_rb = np.int64(np.mean(rotations_insertion, axis=1))
avg_heights_before_rb = np.mean(heights_before_deletion, axis=1)
avg_heights_after_rb = np.mean(heights_after_deletion, axis=1)
avg_rotations_deletion_rb = np.int64(np.mean(rotations_deletion, axis=1))

print("Average Rotations for Insertions:-redblack tree : ", avg_rotations_insertion_rb)
print("Average Heights Before Deletion:-redblack tree : ", avg_heights_before_rb)
print("Average Heights After Deletion:-redblack tree : ", avg_heights_after_rb)
print("Average Rotations for Deletions:-redblack tree :", avg_rotations_deletion_rb)


filename = '/home/vishnu/assignments/avl_tree_output.txt'


rotations_insertion,rotations_deletion, heights_before_deletion, heights_after_deletion  = read_data(filename)

rotations_insertion = rotations_insertion.reshape(4, -1)
heights_before_deletion = heights_before_deletion.reshape(4, -1)
heights_after_deletion = heights_after_deletion.reshape(4, -1)
rotations_deletion = rotations_deletion.reshape(4, -1)

avg_rotations_insertion_av = np.int64(np.mean(rotations_insertion, axis=1))
avg_heights_before_av = np.mean(heights_before_deletion, axis=1)
avg_heights_after_av = np.mean(heights_after_deletion, axis=1)
avg_rotations_deletion_av = np.int64(np.mean(rotations_deletion, axis=1))

print("Average Rotations for Insertions:-avl tree : ", avg_rotations_insertion_av)
print("Average Heights Before Deletion:-avl tree : ", avg_heights_before_av)
print("Average Heights After Deletion:-avl tree : ", avg_heights_after_av)
print("Average Rotations for Deletions:-avl tree :", avg_rotations_deletion_av)

x=["10^4","10^5","10^6","10^7"]

# plt.ticklabel_format(useOffset=False, style='plain', axis='y')


plt.bar(x,avg_heights_before_av,label="avl tree",color='#687bf7')
plt.bar(x,avg_heights_before_rb,label="rb tree",color='#f2963f')
plt.xlabel("array sizes")
plt.ylabel("heights")
plt.title("Average Heights Before Deletion")
plt.legend(loc='upper right', bbox_to_anchor=(0.5, 1.15), ncol=4)

plt.savefig("Average_Heights_Before_Deletion.png")
plt.clf()

plt.bar(x,avg_heights_after_av,label="avl tree",color='#687bf7')
plt.bar(x,avg_heights_after_rb,label="rb tree",color='#f2963f')
plt.xlabel("array sizes")
plt.ylabel("heights")
plt.title("Average Heights after Deletion")
plt.legend(loc='upper right', bbox_to_anchor=(0.5, 1.15), ncol=4)

plt.savefig("Average_Heights_after_Deletion.png")
plt.clf()

plt.bar(x,avg_rotations_insertion_av,label="avl tree",color='#687bf7')
plt.bar(x,avg_rotations_insertion_rb,label="rb tree",color='#f2963f')

plt.xlabel("array sizes")
plt.ylabel("rotations")
plt.title("Average Rotations for Insertions")
plt.legend(loc='upper right', bbox_to_anchor=(0.5, 1.15), ncol=4)

plt.savefig("Average_Rotations_for_Insertions.png")
plt.clf()

plt.bar(x,avg_rotations_deletion_av,label="avl tree",color='#687bf7')
plt.bar(x,avg_rotations_deletion_rb,label="rb tree",color='#f2963f')

plt.xlabel("array sizes")
plt.ylabel("rotations")
plt.title("Average Rotations for deletions")
plt.legend(loc='upper right', bbox_to_anchor=(0.5, 1.15), ncol=4)

plt.savefig("Average_Rotations_for_deletions.png")
# print(rotations_insertion, heights_after_deletion, heights_before_deletion , rotations_deletion)