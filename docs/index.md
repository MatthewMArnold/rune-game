---
# Feel free to add content and custom Front Matter to this file.
# To modify the layout, see https://jekyllrb.com/docs/themes/#overriding-theme-defaults

title: Home
layout: template
filename: index.html
---

<!-- Problem description
Previous work (including what you used for your method i.e. pretrained models)
Your approach
Datasets
Results
Discussion
    What problems did you encounter?
    Are there next steps you would take if you kept working on the project?
    How does your approach differ from others? Was that beneficial? -->

## The Problem

The RoboMaster robotics competition is an arena style competition where ~400 universities from
around the world build a squad of robots and compete head-to-head against each other. A major
component of this competition is a challenge called the "rune game." Your robot must hit a sequence
of plates that light up in a random order. The plates are located at a considerable distance (8m)
away and the rune game spins--initially at a constant rotational speed and then at a nonconstant
rotational speed as the match progresses. This requires that your robot can automatically detect and
aim at specific plates.

![](./resources/rune_game_graphic.gif)

## Previous Work, Datasets, and Approach

**See the [Classical CV Approach](classical-cv.html) approach or [ML Approach](ml-approach.html).**

## Discussion

### Problems

One of the major problems that we encountered and that resulted in us starting over on the classical
CV approach was that our detection technique was too brittle. By only attempting to detect a single
feature, our detection pipeline could be easily fooled or would misdetect. The second attempt at the
problem turned out significantly better, than the first, yet there are certainly weaknesses to using
classical CV for this problem. Most notably is that parameters tuned in our testing setup will not
transfer well to the real competition. This can be solved by testing the rune game detector in real
life on a platform that is as close to the competition as possible. While a classical CV approach
does not have great transferability, it is unlikely that ML approaches that are overtrained on a
dataset will work well on a dataset the model has not encountered before.

### Next Steps

There are a lot of improvements and future work we would like to do on this project. First off, we
would like to run the rune game detectors we have developed on a real rune game instead of just
using images from previous competitions. This will allow us to find more weaknesses in the detectors
and will help us better tune them.

We would also like to combine ML and classical techniques as described in "Detection and
Classification of Geometric Shape Objects for Industrial Applications". We believe this technique of
classifying binarized images would work well for the rune game detection problem. The binarization
portion of the pipeline could be quickly tuned to perform the correct color/brightness thresholding
necessary to generate a reasonably binarized image, and the ML model would not be affected by
lighting differences and would be more resiliant than manually performing geometric thresoling to
pick out different features. Furthremore, the ML model would be directly transferrible between a
test environment and the actual competition environment. The only reason we did not take this
approach was we did not have labeled binarized images that we could use to train an ML model and the
time constraint inhibited us from labeling a bunch of data.

Another future improvement we would like to make to the classical CV pipeline is add python bindings
to the detector such that it can rune in a python project. This will improve the extensibility of
the project and allow for interchangability between the ML and classical CV pipelines when we
integrate one of the detectors into our vision pipeline. This is somewhat outside the scope of this
project but is going to be important moving forward as we integrate the rune game detector with
other software.

### Reflection/Comparing our Work

TODO
