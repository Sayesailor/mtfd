MTFD is abbreviation of multi-thread face detector, the tool is developed on a face detector
There are parameters for function setting early, with count of parameter increase, we use config file.
The form of MTFD calling is:

./mtfd src_path crop_outpath result_outpath img_suff thread_count multi_face enable_crop  model_path log_path mean_shape final_size pthfn_lst

src_path         source path of images for processing
crop_path        destination path of crop image output
dest_path        destination path of detection and 5-point position output
img_suff         suffix name of image file for processing
thread_cnt       working thread count
multi_face
face_crop
model_path
log_path
mean_shape
final_size
pthfn_lst


content of conf.json is below
--------------------------------
{
	"src_path"   : "/tmp/data/",
	"crop_path"  : "/tmp/crop/",
	"dest_path"  : "/tmp/output/",
	"img_suffix" : "png",
	"thread_cnt" : 2,
	"multi_face" : 1,
	"face_crop"  : 1,
	"model_path" : "/tmp/seeta/model/",
	"log_path"   : "/tmp/seeta/log/",
	"mean_shape" : 256,
	"final_size" : 256,
	"pthfn_lst"  : ""
}

#NOTE: we need mkdir crop destination path structure same with source path
#this line create same directory struct with src_path
find $1 -type d | sed 's?^?\mkdir -p "?' | sed 's?$?\"?' | sed "s?$1?$2?" | sh
test -d dest_path || mkdir -p dest_path

