from .libmtfd import mtfd_proc

def mtfd_crop_image(src_path,
                    crop_outpath,
                    five_point_outpath,
                    img_lst_file,
                    mean_shape=256,
                    final_size=256,
                    detect_model_path='',
                    thread_count=4,
                    log_path='/dev/null',
                    multi_face=False,
                    enable_crop=True):
     assert isinstance(src_path, str) and \
      isinstance(crop_outpath, str) and \
      isinstance(five_point_outpath, str) and \
      isinstance(mean_shape, int) and \
      isinstance(final_size, int) and \
      isinstance(detect_model_path, str) and \
      isinstance(thread_count, int) and \
      isinstance(log_path, str) and \
      isinstance(multi_face, bool) and \
      isinstance(enable_crop, bool)
     mtfd_proc(src_path, 
               crop_outpath, 
               five_point_outpath, 
               str(thread_count), 
               str(int(multi_face)), 
               str(int(enable_crop)), 
               detect_model_path, 
               log_path, 
               str(mean_shape), 
               str(final_size), 
               img_lst_file)
