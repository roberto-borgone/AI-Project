"""**Import libraries**"""

import os
import os.path
import sys

from torchvision.datasets import VisionDataset

from PIL import Image


"""# **Set Arguments**"""

DATA_DIR = 'AIML_project/dataset1'


IMG_EXTENSIONS = [
    '.jpg', '.JPG', '.jpeg', '.JPEG',
    '.png', '.PNG', '.ppm', '.PPM', '.bmp', '.BMP',
]

def pil_loader(path):
    # open path as file to avoid ResourceWarning (https://github.com/python-pillow/Pillow/issues/835)
    with open(path, 'rb') as f:
        img = Image.open(f)
        return img.convert('RGB')

class PACS(VisionDataset):
  def __init__(self, root, split='train', transform=None, target_transform=None, loader=pil_loader):
        super(PACS, self).__init__(root, transform=transform, target_transform=target_transform)
        
        self.transform = transform
        self.target_transform = target_transform
        self.loader = loader
        self.classes, self.class_to_idx = self.find_classes(self.root)
        self.images = self.make_dataset(DATA_DIR,self.class_to_idx)

  def _find_classes(self, dir):
        
        if sys.version_info >= (3, 5):
            # Faster and available in Python 3.5 and above
            classes = [d.name for d in os.scandir(dir) if d.is_dir()]
        else:
            classes = [d for d in os.listdir(dir) if os.path.isdir(os.path.join(dir, d))]
        classes.sort()
        class_to_idx = {classes[i]: i for i in range(len(classes))}
        return classes, class_to_idx

  def make_dataset(self, dir, class_to_idx):
    images = []
    dir = os.path.expanduser(dir)
    
    for target in sorted(class_to_idx.keys()):
        d = os.path.join(dir, target)
        if not os.path.isdir(d):
            continue
        for root, dirs, _ in sorted(os.walk(d)):
            for i in sorted(dirs):
                path = os.path.join(root, i)
                item = (path, class_to_idx[target])
                images.append(item)

    return images  # contiene i path delle cartelle contenenti le quaterne. Ogni path è associato ad un'etichetta che indica la posizione dell'odd

  def __getitem__(self, index):

        quad = []
        sample_dir, label = self.images[index]
        for img in sorted(os.listdir(sample_dir)):
          image_path = os.path.join(sample_dir, img) 
          quad.append( self.loader(image_path))

        # Applies preprocessing when accessing the image
        if self.transform is not None:
          for i in range(4):
              quad[i] = self.transform(quad[i])
        if self.target_transform is not None:
            label = self.target_transform(label)

        return quad, label  

  def __len__(self):
        length = len(self.images)
        return length
