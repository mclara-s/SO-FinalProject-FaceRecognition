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
pessoas_antes = 0  

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
        pessoas = int(format(len(faces)))

        if pessoas_antes != pessoas:
            pessoas_antes = pessoas
            mudar = '1'
        else:
            mudar = '0'


        temp = 27.3 - ((0.3)*pessoas)

        if(temp < 18):
            temp = 18
        if(pessoas == 0):
            temp = 30
        temp = int(temp)
        txt.write(mudar+"|"+str(temp))
        txt.close()
        # print "Found {0} faces!".format(len(faces))
    # end timer

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# When everything is done, release the capture
video_capture.release()
cv2.destroyAllWindows()
