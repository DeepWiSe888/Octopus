# SKLearn Implementations in Embedded System with C Language



##### sklearn-porter

1. Dependencies
```
pip install sklearn-porter
```
2. Export Model File

```
from sklearn.datasets import load_iris
from sklearn.tree import tree
from sklearn.svm import SVC
from sklearn_porter import Porter

# Load data and train the classifier:
samples = load_iris()
X, y = samples.data, samples.target
clf = svc()
clf.fit(X, y)

# Export:
porter = Porter(clf, language='java')
output = porter.export(embed_data=True)
print(output)
with open('svm_model.txt', 'w') as f:
    f.write(output)
```
3. You also can export in command line tool, by joblib.   But latest sklearn no longer support the jobib, you need to modify the .py file as below:
File Name “$sklearn_porter\cli\__main__.py”, fix it as

```
# from sklearn.externals import joblib
import joblib
```
Now you use command line to generate c file:

```python
#use joblib to dump model file
joblib.dump(clf, 'estimator.pkl', compress=0)

porter estimator.pkl --c

#generate c file
porter estimator.pkl --c --pipe > estimator.c
```


4. The c file includes module content and predict interface 

```c
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


int predict (float features[]) {
    int i, j, k, d, l;
    classIdx = 0;
    // ...
    return classIdx;

}

int main(int argc, const char * argv[]) {

    /* Features: */
    float features[argc-1];
    int i;
    for (i = 1; i < argc; i++) {
        features[i-1] = atof(argv[i]);
    }

    /* Prediction: */
    printf("%d", predict(features));
    return 0;

}
```