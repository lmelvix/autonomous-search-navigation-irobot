from neon import logger as neon_logger
from neon.data import CIFAR10
from neon.initializers import Uniform
from neon.layers import GeneralizedCost, Affine
from neon.models import Model
from neon.optimizers import GradientDescentMomentum
from neon.transforms import Misclassification, CrossEntropyBinary, Logistic, Rectlin
from neon.callbacks.callbacks import Callbacks
from neon.util.argparser import NeonArgparser

from neon.data.dataiterator import ArrayIterator

from neon.layers import Conv, Pooling
from neon.transforms.activation import Softmax
from neon.transforms import CrossEntropyMulti
from neon.optimizers import RMSProp


from neon.backends import gen_backend

be = gen_backend()

# parse the command line arguments
parser = NeonArgparser(__doc__)
args = parser.parse_args()

dataset = CIFAR10(path=args.data_dir,
                  normalize=True,
                  contrast_normalize=False,
                  whiten=False)
train = dataset.train_iter
test = dataset.valid_iter

init_uni = Uniform(low=-0.1, high=0.1)

opt_gdm = GradientDescentMomentum(learning_rate=0.005, momentum_coef=0.9)

# set up the model layers
#layers = [Affine(nout=200, init=init_uni, activation=Rectlin()),Affine(nout=10, init=init_uni, activation=Logistic(shortcut=True))]
init_uni = Uniform(low=-0.1, high=0.1)
layers = [Conv(fshape=(5,5,16), init=init_uni, activation=Rectlin()),
          Pooling(fshape=2, strides=2),
          Conv(fshape=(5,5,32), init=init_uni, activation=Rectlin()),
          Pooling(fshape=2, strides=2),
          Affine(nout=500, init=init_uni, activation=Rectlin()),
          Affine(nout=10, init=init_uni, activation=Softmax())]

cost = GeneralizedCost(costfunc=CrossEntropyMulti())

mlp = Model(layers=layers)

# configure callbacks
callbacks = Callbacks(mlp, eval_set=test, **args.callback_args)

mlp.fit(train, optimizer=opt_gdm, num_epochs=args.epochs,
        cost=cost, callbacks=callbacks)

neon_logger.display('Misclassification error = %.1f%%' %
                    (mlp.eval(test, metric=Misclassification()) * 100))


# an image of a frog from wikipedia
img_source = "https://upload.wikimedia.org/wikipedia/commons/thumb/5/55/Atelopus_zeteki1.jpg/440px-Atelopus_zeteki1.jpg"

# download the image
import urllib
urllib.urlretrieve(img_source, filename="image.jpg")

# crop and resize to 32x32
from PIL import Image
import numpy as np

img = Image.open('image.jpg')
crop = img.crop((0,0,min(img.size),min(img.size)))
crop.thumbnail((32, 32))
crop = np.asarray(crop, dtype=np.float32)

import numpy as np
x_new = np.zeros((128,3072), dtype=np.float32)
x_new[0] = crop.reshape(1,3072)/ 255

inference_set =ArrayIterator(X=x_new, y=None, nclass=10, lshape=(3, 32, 32))

classes =["airplane", "automobile", "bird", "cat", "deer",
          "dog", "frog", "horse", "ship", "truck"]
out = mlp.get_outputs(inference_set)
print classes[out[0].argmax()]


