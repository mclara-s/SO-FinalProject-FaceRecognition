import cv2
import sys
# import time
# from threading import Timer

cascPath = sys.argv[1]
faceCascade = cv2.CascadeClassifier(cascPath)

video_capture = cv2.VideoCapture(0)

# def olar():
#     print "Found {0} faces!".format(len(faces))
#     t = Timer(3, olar)
#     t.start()   

# t = Timer(3, olar)
# t.start() 

counter = -1  

while True:
    # Capture frame-by-frame
    

    ret, frame = video_capture.read()

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    faces = faceCascade.detectMultiScale(
        gray,
        scaleFactor=1.2,
        minNeighbors=4,
        minSize=(30, 30),
        flags=cv2.cv.CV_HAAR_SCALE_IMAGE
    )

    # Draw a rectangle around the faces
    for (x, y, w, h) in faces:
        cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)

    # Display the resulting frame
    cv2.imshow('Video', frame)

    # timer para imprimir    
    counter += 1
    counter = counter%20

    if counter == 0:
        txt = open('faces', 'w')
        txt.write("Found {0} faces!".format(len(faces)))
        txt.close()
        # print "Found {0} faces!".format(len(faces))
    # end timer

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# When everything is done, release the capture
video_capture.release()
cv2.destroyAllWindows()
