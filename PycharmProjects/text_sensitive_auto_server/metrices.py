from sklearn.metrics import f1_score, accuracy_score
import numpy as np
def flat_accuracy(preds, labels):
    """A function for calculating accuracy scores"""
    pred_flat = np.argmax(preds, axis=1).flatten()
    labels_flat = labels.flatten()
    return f1_score(labels_flat, pred_flat)