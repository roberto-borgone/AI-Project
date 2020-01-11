import random
import os
from PIL import Image, ImageColor
import numpy as np
import copy as cp


def permutation():
    
    i=0
    j=1
    k=2
    
    while i==0 and j==1 and k==2:
        i=random.choice([0,1,2])
        j=random.choice([0,1,2])
        while j==i:
            j=random.choice([0,1,2])
        k=random.choice([0,1,2])
        while k==i or k==j:
            k=random.choice([0,1,2])
    
    return i,j,k
    
    
cartoon ='C:\\Users\\Marianna\\Desktop\\AIML_project\\AIML_project\\cartoon'
dataset = 'C:\\Users\\Marianna\\Desktop\\coloured datasets'
dog_path = cartoon+'\dog'
elephant_path = cartoon+'\elephant'
giraffe_path = cartoon+'\giraffe'
guitar_path = cartoon+'\guitar'
horse_path = cartoon+'\horse'
house_path = cartoon+'\house'
person_path = cartoon+'\person'

paths = [dog_path, elephant_path, giraffe_path, guitar_path, 
         horse_path, house_path, person_path]

count1 = 0
count2 = 0
count3 = 0
count4 = 0
        
for path1 in paths:
    
    path_len1 = len(os.listdir(path1))
    indx = int(path_len1/4)
    print(path_len1)
    
    count = 0

    for img_file in os.listdir(path1):
        odd = Image.open(path1 + '/' + img_file)
        img_orig = cp.copy(odd)
        #im1.show()
        pixels = odd.load()
        width, height = odd.size
        
        first = True
        ch=np.zeros(3)
        i=j=k=0
        for y in range(height):
            for x in range(width):
                r,g,b=pixels[x,y]
                if first:
                    i,j,k=permutation() # compute permutation of rgb
                    first=False         # The permutation will be the same for 
                                        # the whole image
                ch[i]=r
                ch[j]=g
                ch[k]=b
                
                pixels[x,y]=(int(ch[0]),int(ch[1]),int(ch[2]))
        #odd.show()
        #img_orig.show()
        
    
        count = count + 1
    
        if count<=indx:
            # odd = immagine 1
            count1 = count1 + 1
            label1_path = dataset+'\\1'
            
            if not os.path.exists(label1_path):
                os.mkdir(label1_path)
                
            label1_path = label1_path+'\\'+str(count1) # folder name of this dataset entry
            if not os.path.exists(label1_path):
                os.mkdir(label1_path)
                
            odd.save(label1_path+'\\1.jpg')
            img_orig.save(label1_path+'\\2.jpg')
            img_orig.save(label1_path+'\\3.jpg')
            img_orig.save(label1_path+'\\4.jpg')
            # break        
        elif count>indx and count<=indx*2:
            # odd = immagine 2
            count2 = count2 + 1
            label2_path = dataset+'\\2'
            
            if not os.path.exists(label2_path):
                os.mkdir(label2_path)
                
            label2_path = label2_path+'\\'+str(count2)
            if not os.path.exists(label2_path):
                os.mkdir(label2_path)
                
            img_orig.save(label2_path+'\\1.jpg')
            odd.save(label2_path+'\\2.jpg')
            img_orig.save(label2_path+'\\3.jpg')
            img_orig.save(label2_path+'\\4.jpg')

        elif count>indx*2 and count<=indx*3:
            # odd = immagine 3
            count3 = count3 + 1
            label3_path = dataset+'\\3'
            
            if not os.path.exists(label3_path):
                os.mkdir(label3_path)
                
            label3_path = label3_path +'\\'+str(count3)
            if not os.path.exists(label3_path):
                os.mkdir(label3_path)
                
            img_orig.save(label3_path+'\\1.jpg')
            img_orig.save(label3_path+'\\2.jpg')
            odd.save(label3_path+'\\3.jpg')
            img_orig.save(label3_path+'\\4.jpg')

        else:
            # odd = immagine 4
            count4 = count4 + 1
            label4_path = dataset+'\\4'
            
            if not os.path.exists(label4_path):
                os.mkdir(label4_path)            
            
            label4_path = label4_path +'\\'+str(count4)
            if not os.path.exists(label4_path):
                os.mkdir(label4_path)
                
            img_orig.save(label4_path+'\\1.jpg')
            img_orig.save(label4_path+'\\2.jpg')
            img_orig.save(label4_path+'\\3.jpg')
            odd.save(label4_path+'\\4.jpg')
    #break