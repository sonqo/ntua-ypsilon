import cv2 as cv
import numpy as np
from skimage.morphology import disk
from skimage.util import random_noise
from skimage import filters, img_as_ubyte

# accept two point coordinates and check if their distance is more than 1 in one direction
def dist_big_enough(a, b):
    if abs(int(a[0]) - int(b[0])) > 1 or abs(int(a[1]) - int(b[1])) > 1:
        return True
    return False

# compute amount variable for salt and pepper noise
def snp_amount(x):
    return 0.3 + x / 90

# image is passed as ubyte array
def add_snp(image, s=8, x=3):
    # add salt and pepper noise
    noise = random_noise(image, mode='s&p', seed=s, amount=snp_amount(x))
    # convert noise_image to same type as the initial
    result = img_as_ubyte(noise)
    return result

# return image with half of the original resolution
def resize(image):
    height = image.shape[0] // 2
    width = image.shape[1] // 2
    resized = cv.resize(image, dsize=(width, height))
    return resized

def produce_corners(image):
    # store results of corner detection in list, separately for each algorithm
    corners_st = []
    corners_h = []
    titles = []
    # loop through values for each variable that we investigate
    for n in [200, 400, 500, 600]:
        for q in [6, 8, 10, 14]:
            for d in [6, 8, 10, 14]:
                # for each parameter configuration apply corner detection
                param = dict(maxCorners=n, qualityLevel=q/100, minDistance=d)
                # save config params to print in the title
                title = "max_corners="+str(n)+", quality="+str(q/100)+", min_dist="+str(d)
                titles.append(title)
                # apply Shi-Tomashi
                corners_st.append(cv.goodFeaturesToTrack(image, useHarrisDetector=False, **param))
                # apply Harris
                corners_h.append(cv.goodFeaturesToTrack(image, useHarrisDetector=True, **param))
    return corners_st, corners_h, titles

def part3(filename):
    video = cv.VideoCapture(filename)
    ret, fr = video.read()
    if ret is False:
        print("Invalid Input File\n")
        exit(0)
    #resize frame
    frame = resize(fr)

    # convert to grayscale for corner detection
    gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)

    #produce different corner detection results
    c_st, c_h, config = produce_corners(gray)

    # show results for each algorithm
    for st, h, conf in zip(c_st, c_h, config):
        titles = ["Shi-Tomashi: "+str(conf), "Harris: "+str(conf)]
        for corners, title in zip([st, h], titles):
            #create a mask with points of interest
            mask = np.zeros_like(frame)
            for i, point in enumerate(corners):
                x, y = point.ravel()
                cv.circle(mask, (x, y), 2, (0, 255, 0), -1)
            #cv.imwrite(title+".png", cv.add(frame, mask)); continue
            cv.imshow(title, cv.add(frame, mask))
            cv.moveWindow("Shi-Tomashi: "+str(conf), 200, 200)
            cv.moveWindow("Harris: "+str(conf), 1000, 200)
        ch = cv.waitKey(0)
        cv.destroyAllWindows()
        if ch == 113:
            break

def part4(filename):
    video = cv.VideoCapture(filename)
    # different colors for Shi-Tomashi and Harris corner detector
    color = (0, 255, 0)
    ret, fr = video.read()
    video.release()
    if ret is False:
        print("Invalid Input File\n")
        exit(0)
    # resize frame
    frame = resize(fr)
    #covnert to grayscale and find feature points for the first frame
    gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
    c_st, h_st, conf = produce_corners(gray)

    #first loop's previous gray image will be the first
    prev_gray = gray.copy()
    # loop through diffent configurations for Lucas-Kanade
    for win_size in [3, 9, 15]:
        for max_l in [1, 3, 5]:
            # declare current config
            lk_par = dict(winSize=(win_size, win_size), maxLevel=max_l, criteria=(cv.TERM_CRITERIA_EPS | cv.TERM_CRITERIA_COUNT, 10, 0.01))
            # loop through corner detection configurations
            for p_st, p_h, title in zip(c_st, h_st, conf):

                #open video again to compute sparse optical flow
                video = cv.VideoCapture(filename)
                # keep list with lists of the positions for every point that we track (both algorithms)
                tracks_l = [[],[]]
                for x, y in np.float32(p_st).reshape(-1, 2):
                    tracks_l[0].append([(x, y)])
                for x, y in np.float32(p_h).reshape(-1, 2):
                    tracks_l[1].append([(x, y)])

                # calculate optical flow loop
                while video.isOpened():
                    ret, fr = video.read()
                    if ret is False:
                        break
                    # resize frame and convert to grayscale
                    frame = resize(fr)
                    gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
                    out_image = [frame.copy(), frame.copy()]

                    # for each corner detection algorithm
                    for tracks, alg, out_im in zip(tracks_l, ["Shi-Tomashi", "Harris"], out_image):
                        # retrive last position of tracked points and store the coordinates in prev_points
                        p_before = np.float32([tr[-1] for tr in tracks]).reshape(-1, 1, 2)

                        # calculate sparse optical flow using Lucas-Kanade
                        p_after, status, err = cv.calcOpticalFlowPyrLK(prev_gray, gray, p_before, None, **lk_par)

                        # apply algorithm again backwards from new to old image for the new points position
                        p_backtrace, status, err = cv.calcOpticalFlowPyrLK(gray, prev_gray, p_after, None, **lk_par)

                        # calculate difference between old  potition and backtraced position of each point
                        dif = abs(p_before - p_backtrace).reshape(-1, 2).max(-1)
                        # create status flag to discard false detection of momvement in similar/uniform areas
                        status = dif < 1

                        # create a new list and store valid tracks
                        new_tracks = []
                        # for each tracked feature point
                        for tr, (x, y), status in zip(tracks, p_after.reshape(-1, 2), status):
                            if status:  # if point is valid
                                # add new potition to point's flow
                                tr.append((x, y))
                                # reduce length of the stored potitions (keep last <track_len> potitions)
                                while len(tr) > track_length:
                                    del tr[0]
                                # store point's potitions in new tracks
                                new_tracks.append(tr)
                        # update tracks
                        tracks = new_tracks
                        # update track_l
                        if alg=="Harris":
                            tracks_l[1] = tracks
                        else:
                            tracks_l[0] = tracks

                        # draw movement for tracked points
                        for tr in tracks:
                            # check last and first and last known position are different before drawing
                            cv.circle(out_im, tr[-1], 2, color, -1)
                            cv.polylines(out_im, [np.int32(tr)], False, color)

                        t = str(alg)+" "+title+" lk("+str(win_size)+","+str(max_l)+")"
                        cv.imshow(t, out_im)
                        cv.moveWindow("Shi-Tomashi"+" "+title+" lk("+str(win_size)+","+str(max_l)+")", 200, 200)
                        cv.moveWindow("Harris"+" "+title+" lk("+str(win_size)+","+str(max_l)+")", 1000, 200)

                    prev_gray = gray
                    ch = cv.waitKey(1)

                    if ch == 8 or ch == 27 or ch == 113:
                        break
                video.release()
                cv.destroyAllWindows()
                if ch == 113:
                    exit(0)
                elif ch == 27:
                    break

def part5(filename):
    video = cv.VideoCapture(filename)
    color = (0, 255, 0)

    # keep track of frame number for feature points refresh
    frame_index = 0
    # Keep data for each corner detection algorithm separately
    harris_flag = [False, True]  #first Shi-Tomashi then Harris, passed to corner detection function

    # for each point of interest use a list to store the coordinates of its flow
    # each point has its own flow and its list is an element of the tracks list
    # so the tracks list contrains the flow of all tracked points
    tracks_l = [[], []]  # start with zero points

    # video title
    titles = ["Shi-Tomashi", "Harris"]

    # cornrer detector parameters
    feature_params = [st_params, h_params]

    while True:
        # read next frame and check if we reach the end
        ret, fr = video.read()
        if ret is False:
            if frame_index == 0:
                print("Invalid Input File\n")
            break

        # resize and keep copy for the output image
        frame = resize(fr)
        out_image = [frame.copy(), frame.copy()]

        # convert to grayscale for corner and flow detection
        gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)

        # repeat flow detection for bot corner detection algorithms
        for tracks, h_bit, title, out_im, corner_params in zip(tracks_l, harris_flag, titles, out_image, feature_params):
            if len(tracks) > 0:  # if we have any points of interest calculate optical flow

                # retrive last position of tracked points and store the coordinates in prev_points
                p_before = np.float32([tr[-1] for tr in tracks]).reshape(-1, 1, 2)

                # calculate sparse optical flow using Lucas-Kanade
                p_after, status, err = cv.calcOpticalFlowPyrLK(prev_gray, gray, p_before, None, **lk_params)

                # apply algorithm again backwards from new to old image for the new points position
                p_backtrace, status, err = cv.calcOpticalFlowPyrLK(gray, prev_gray, p_after, None, **lk_params)

                # calculate difference between old  potition and backtraced position of each point
                dif = abs(p_before - p_backtrace).reshape(-1, 2).max(-1)
                # create status flag to discard false detection of momvement in similar/uniform areas
                status = dif < 1

                # create a new list and store valid tracks
                new_tracks = []
                # for each tracked feature point
                for tr, (x, y), status in zip(tracks, p_after.reshape(-1, 2), status):
                    if status:  # if point is valid
                        # add new potition to point's flow
                        tr.append((x, y))
                        # reduce length of the stored potitions (keep last <track_len> potitions)
                        while len(tr) > track_length:
                            del tr[0]
                        # store point's potitions in new tracks
                        new_tracks.append(tr)
                # update tracks
                tracks = new_tracks
                # update track_l
                if h_bit:
                    tracks_l[1] = tracks
                else:
                    tracks_l[0] = tracks
                # draw movement for tracked points
                for tr in tracks:
                    # check last and first and last known position are different before drawing
                    if dist_big_enough(tr[0], tr[-1]):
                        cv.circle(out_im, tr[-1], 2, color, -1)
                        cv.polylines(out_im, [np.int32(tr)], False, color)
                    # remove not moving points from the tracked list
                    elif len(tr) >= track_length:
                        tracks.remove(tr)

            # check for new potential featured points every <refresh_interval> frames
            if frame_index % refresh_interval == 0:
                #create a mas to exclude already tracked points from new corner detection
                mask = np.zeros_like(gray)
                mask[:] = 255
                # "white-out" tracked points' position in the mask-array
                for x, y in [np.int32(tr[-1]) for tr in tracks]:
                    cv.circle(mask, (x, y), 20, 0, -1)
                # find new point and add them to tre tracked
                new_corners = cv.goodFeaturesToTrack(gray, mask=mask, useHarrisDetector=h_bit, **corner_params)
                for x, y in np.float32(new_corners).reshape(-1, 2):
                    tracks.append([(x, y)])

            # show next image of video feed (performed for both algorithms)
            cv.imshow(title, out_im)
            cv.moveWindow("Shi-Tomashi", 200, 200)
            cv.moveWindow("Harris", 1000, 200)
        #increment frame counter and update previous gray image to current new
        frame_index += 1
        prev_gray = gray

        # add small delay between frame outputs
        ch = cv.waitKey(20)
        #if "q" is pressed quit
        if ch == 113:
            break
    # close video and all windows
    video.release()
    cv.destroyAllWindows()

def part6(filename):
    video = cv.VideoCapture(filename)
    color = (0, 255, 0)

    # keep track of frame number for feature points refresh
    frame_index = 0
    #### Keep data for each corner detection algorithm separately
    harris_flag = [False, True] #first Shi-Tomashi then Harris, passed to corner detection function

    # for each point of interest use a list to store the coordinates of its flow
    # each point has its own flow and its list is an element of the tracks list
    # so the tracks list contrains the flow of all tracked points
    tracks_l = [[], []]  # start with zero points

    # video title
    titles = ["Shi-Tomashi", "Harris"]

    # cornrer detector parameters
    feature_params = [st_params, h_params]

    while True:
        # read next frame and check if we reach the end
        ret, fr = video.read()
        if ret is False:
            if frame_index == 0:
                print("Invalid Input File\n")
            break

        # resize and keep copy for the output image
        frame = resize(fr)
        out_image = [frame.copy(), frame.copy()]

        # convert to grayscale for corner and flow detection
        gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
        # add noise to grayscale image before corner detection and motion estimation
        gray = add_snp(gray)

        # repeat flow detection for bot corner detection algorithms
        for tracks, h_bit, title, out_im, corner_params in zip(tracks_l, harris_flag, titles, out_image, feature_params):
            if len(tracks) > 0:  # if we have any points of interest calculate optical flow

                # retrive last position of tracked points and store the coordinates in prev_points
                p_before = np.float32([tr[-1] for tr in tracks]).reshape(-1, 1, 2)

                # calculate sparse optical flow using Lucas-Kanade
                p_after, status, err = cv.calcOpticalFlowPyrLK(prev_gray, gray, p_before, None, **lk_params)

                # apply algorithm again backwards from new to old image for the new points position
                p_backtrace, status, err = cv.calcOpticalFlowPyrLK(gray, prev_gray, p_after, None, **lk_params)

                # calculate difference between old  potition and backtraced position of each point
                dif = abs(p_before - p_backtrace).reshape(-1, 2).max(-1)
                # create status flag to discard false detection of momvement in similar/uniform areas
                status = dif < 1

                # create a new list and store valid tracks
                new_tracks = []
                # for each tracked feature point
                for tr, (x, y), status in zip(tracks, p_after.reshape(-1, 2), status):
                    if status:  # if point is valid
                        # add new potition to point's flow
                        tr.append((x, y))
                        # reduce length of the stored potitions (keep last <track_len> potitions)
                        while len(tr) > track_length:
                            del tr[0]
                        # store point's potitions in new tracks
                        new_tracks.append(tr)
                # update tracks
                tracks = new_tracks
                if h_bit:
                    tracks_l[1] = tracks
                else:
                    tracks_l[0] = tracks

                # draw movement for tracked points
                for tr in tracks:
                    # check last and first and last known position are different before drawing
                    if dist_big_enough(tr[0], tr[-1]):
                        cv.circle(out_im, tr[-1], 2, color, -1)
                        cv.polylines(out_im, [np.int32(tr)], False, color)
                    # remove not moving points from the tracked list
                    elif len(tr) >= track_length:
                        tracks.remove(tr)

            # check for new potential featured points every <refresh_interval> frames
            if frame_index % refresh_interval == 0:
                #create a mas to exclude already tracked points from new corner detection
                mask = np.zeros_like(gray)
                mask[:] = 255
                # "white-out" tracked points' position in the mask-array
                for x, y in [np.int32(tr[-1]) for tr in tracks]:
                    cv.circle(mask, (x, y), 8, 0, -1)
                # find new point and add them to tre tracked
                new_corners = cv.goodFeaturesToTrack(gray, mask=mask, useHarrisDetector=h_bit, **corner_params)
                for x, y in np.float32(new_corners).reshape(-1, 2):
                    tracks.append([(x, y)])

            # show next image of video feed (performed for both algorithms)
            cv.imshow(title, out_im)
            cv.moveWindow("Shi-Tomashi", 200, 200)
            cv.moveWindow("Harris", 1000, 200)
        #increment frame counter and update previous gray image to current new
        frame_index += 1
        prev_gray = gray

        # add small delay between frame outputs
        ch = cv.waitKey(10)
        #if "q" is pressed quit
        if ch == 113:
            break
    # close video and all windows
    video.release()
    cv.destroyAllWindows()

def part7(filename):
    video = cv.VideoCapture(filename)
    color = (0, 255, 0)
    # create convolutions filter for median de-noise
    neighborhood = disk(4)
    # keep track of frame number for feature points refresh
    frame_index = 0
    #### Keep data for each corner detection algorithm separately
    harris_flag = [False, True] #first Shi-Tomashi then Harris, passed to corner detection function

    # for each point of interest use a list to store the coordinates of its flow
    # each point has its own flow and its list is an element of the tracks list
    # so the tracks list contrains the flow of all tracked points
    tracks_l = [[], []]  # start with zero points

    # video title
    titles = ["Shi-Tomashi", "Harris"]

    # cornrer detector parameters
    feature_params = [st_params, h_params]

    while True:
        # read next frame and check if we reach the end
        ret, fr = video.read()
        if ret is False:
            if frame_index == 0:
                print("Invalid Input File\n")
            break

        # resize and keep copy for the output image
        frame = resize(fr)
        out_image = [frame.copy(), frame.copy()]

        # convert to grayscale for corner and flow detection
        gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
        # add noise to grayscale image before corner detection and motion estimation
        noise = add_snp(gray)
        # de-noise grayscale image by applying median filter from 1.4
        gray = filters.rank.median(noise, neighborhood)

        # repeat flow detection for bot corner detection algorithms
        for tracks, h_bit, title, out_im, corner_params in zip(tracks_l, harris_flag, titles, out_image, feature_params):
            if len(tracks) > 0:  # if we have any points of interest calculate optical flow

                # retrive last position of tracked points and store the coordinates in prev_points
                p_before = np.float32([tr[-1] for tr in tracks]).reshape(-1, 1, 2)

                # calculate sparse optical flow using Lucas-Kanade
                p_after, status, err = cv.calcOpticalFlowPyrLK(prev_gray, gray, p_before, None, **lk_params)

                # apply algorithm again backwards from new to old image for the new points position
                p_backtrace, status, err = cv.calcOpticalFlowPyrLK(gray, prev_gray, p_after, None, **lk_params)

                # calculate difference between old  potition and backtraced position of each point
                dif = abs(p_before - p_backtrace).reshape(-1, 2).max(-1)
                # create status flag to discard false detection of momvement in similar/uniform areas
                status = dif < 1

                # create a new list and store valid tracks
                new_tracks = []
                # for each tracked feature point
                for tr, (x, y), status in zip(tracks, p_after.reshape(-1, 2), status):
                    if status:  # if point is valid
                        # add new potition to point's flow
                        tr.append((x, y))
                        # reduce length of the stored potitions (keep last <track_len> potitions)
                        while len(tr) > track_length:
                            del tr[0]
                        # store point's potitions in new tracks
                        new_tracks.append(tr)
                # update tracks
                tracks = new_tracks
                if h_bit:
                    tracks_l[1] = tracks
                else:
                    tracks_l[0] = tracks

                # draw movement for tracked points
                for tr in tracks:
                    # check last and first and last known position are different before drawing
                    if dist_big_enough(tr[0], tr[-1]):
                        cv.circle(out_im, tr[-1], 2, color, -1)
                        cv.polylines(out_im, [np.int32(tr)], False, color)
                    # remove not moving points from the tracked list
                    elif len(tr) >= track_length:
                        tracks.remove(tr)

            # check for new potential featured points every <refresh_interval> frames
            if frame_index % refresh_interval == 0:
                #create a mas to exclude already tracked points from new corner detection
                mask = np.zeros_like(gray)
                mask[:] = 255
                # "white-out" tracked points' position in the mask-array
                for x, y in [np.int32(tr[-1]) for tr in tracks]:
                    cv.circle(mask, (x, y), 8, 0, -1)
                # find new point and add them to tre tracked
                new_corners = cv.goodFeaturesToTrack(gray, mask=mask, useHarrisDetector=h_bit, **corner_params)
                for x, y in np.float32(new_corners).reshape(-1, 2):
                    tracks.append([(x, y)])

            # show next image of video feed (performed for both algorithms)
            cv.imshow(title, out_im)
            cv.moveWindow("Shi-Tomashi", 200, 200)
            cv.moveWindow("Harris", 1000, 200)
        #increment frame counter and update previous gray image to current new
        frame_index += 1
        prev_gray = gray

        # add small delay between frame outputs
        ch = cv.waitKey(1)
        #if "q" is pressed quit
        if ch == 113:
            break
    # close video and all windows
    video.release()
    cv.destroyAllWindows()

filename = "VIRAT_S_010000_08_000893_001024.mp4"

# Shi-Tomashi and Harris chosen parameters
h_params = dict(maxCorners=600, qualityLevel=0.06, minDistance=8, blockSize=7)
st_params = dict(maxCorners=500, qualityLevel=0.08, minDistance=8, blockSize=7)

# Lucas-Kanade parameters
lk_params = dict(winSize=(9, 9), maxLevel=3, criteria=(cv.TERM_CRITERIA_EPS | cv.TERM_CRITERIA_COUNT, 10, 0.01))

# number of last feuture point's positions that we store
track_length = 50
# number of frames between each refresh with corner detection
refresh_interval = 30

# Video download link
# https://data.kitware.com/api/v1/file/56f587848d777f753209cae8/download

if __name__ == '__main__':
    print("You can press 'q' to exit anytime in the process")
    val = input("Choose part to run from 3 to 7: ")
    if val == "3":
        print("Press any key for next configuration or q to exit")
        part3(filename)
    elif val == "4":
        print("Backspace for next corner detection configuration")
        print("Escape for next optical flow configuration")
        print("Press 'q' to exit")
        part4(filename)
    elif val == "5":
        part5(filename)
    elif val == "6":
        part6(filename)
    elif val == "7":
        part7(filename)
    else:
        print("Invalid Input")