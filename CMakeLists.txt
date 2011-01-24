
# Create the extension library
ADD_LIBRARY(Extensions_PropertyBar
  Utils/PropertyBar.h
  Utils/PropertyBar.cpp
)

TARGET_LINK_LIBRARIES(Extensions_PropertyBar
  Extensions_AntTweakBar
  Extensions_PropertyTree
)