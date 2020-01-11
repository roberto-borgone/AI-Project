#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Jan  7 15:38:23 2020

@author: rebecca
"""

import random
import os
from PIL import Image

dog_path = '/home/rebecca/Desktop/ML_project/cartoon/dog'
elephant_path = '/home/rebecca/Desktop/ML_project/cartoon/elephant'
giraffe_path = '/home/rebecca/Desktop/ML_project/cartoon/giraffe'
guitar_path = '/home/rebecca/Desktop/ML_project/cartoon/guitar'
horse_path = '/home/rebecca/Desktop/ML_project/cartoon/horse'
house_path = '/home/rebecca/Desktop/ML_project/cartoon/house'
person_path = '/home/rebecca/Desktop/ML_project/cartoon/person'

paths = [dog_path, elephant_path, giraffe_path, guitar_path, 
         horse_path, house_path, person_path]

count1 = 0
count2 = 0
count3 = 0
count4 = 0
        
for path1 in paths:
    
    path_len = len(os.listdir(path1))
    indx = int(path_len/4)
    print(path_len)
    
    count = 0

    for img_file in os.listdir(path1):
        im = Image.open(path1 + "/" + img_file)
        count = count + 1
    
        for path2 in paths:
            
            if path1 != path2:
                
                path_len = len(os.listdir(path2))
                num = random.randint(1, path_len)
                odd = Image.open(path2 + '/img_' + str(num) + '.jpg')
        
                if count<=indx:
                    # odd = immagine 1
                    count1 = count1 + 1
                    label1_path = '/home/rebecca/Desktop/ML_project/dataset1/1/'+str(count1)
                    
                    if not os.path.exists(label1_path):
                        os.mkdir(label1_path)
                   
                    odd.save(label1_path+'/1.jpg')
                    im.save(label1_path+'/2.jpg')
                    im.save(label1_path+'/3.jpg')
                    im.save(label1_path+'/4.jpg')
        
                elif count>indx and count<=indx*2:
                    # odd = immagine 2
                    count2 = count2 + 1
                    label2_path = '/home/rebecca/Desktop/ML_project/dataset1/2/'+str(count2)
                    
                    if not os.path.exists(label2_path):
                        os.mkdir(label2_path)
                        
                    im.save(label2_path+'/1.jpg')
                    odd.save(label2_path+'/2.jpg')
                    im.save(label2_path+'/3.jpg')
                    im.save(label2_path+'/4.jpg')
        
                elif count>indx*2 and count<=indx*3:
                    # odd = immagine 3
                    count3 = count3 + 1
                    label3_path = '/home/rebecca/Desktop/ML_project/dataset1/3/'+str(count3)
                    
                    if not os.path.exists(label3_path):
                        os.mkdir(label3_path)
                        
                    im.save(label3_path+'/1.jpg')
                    im.save(label3_path+'/2.jpg')
                    odd.save(label3_path+'/3.jpg')
                    im.save(label3_path+'/4.jpg')
        
                else:
                    # odd = immagine 4
                    count4 = count4 + 1
                    label4_path = '/home/rebecca/Desktop/ML_project/dataset1/4/'+str(count4)
                    
                    if not os.path.exists(label4_path):
                        os.mkdir(label4_path)            
                    
                    im.save(label4_path+'/1.jpg')
                    im.save(label4_path+'/2.jpg')
                    im.save(label4_path+'/3.jpg')
                    odd.save(label4_path+'/4.jpg')
    
    