(function(){
function _get_range(arr){
	return arr.reduce(function(p,c){
		return {
			min:"undefined"==typeof(p.min) ? Math.min(p,c) : Math.min(p.min,c),
			max:"undefined"==typeof(p.max) ? Math.max(p,c) : Math.max(p.max,c)
		};
	});
}
function _arr_rgb_hsv(arr){
	var rng = _get_range(arr);
	var h = (rng.max == rng.min ? 0:
		(rng.max == arr[0] ? 60 * (arr[1] - arr[2]) / (rng.max - rng.min):
		(rng.max == arr[1] ? 60 * (arr[2] - arr[0]) / (rng.max - rng.min) + 120:
		60 * (arr[0] - arr[1]) / (rng.max - rng.min))));
	h = Math.round(h < 0 ? h + 360 : h);
	var s = Math.round((rng.max - rng.min) / rng.max * 255)
	return[h, s, rng.max];
}
function _arr_hsv_rgb(arr){
	var mx = arr[2];
	var mn = mx - ((arr[1] / 255) * mx);
	var funcs = [
	    function(){//h:0-60
	    	return [
	    	        mx,
	    	        (arr[0] / 60) * (mx - mn) + nm,
	    	        mn
	    	        ];
	    },
	    function(){//h:60-120
	    	return [
	    	        (120 - arr[0]) / 60 * (mx - mn) + mn,
	    	        mx,
	    	        mn
	    	        ];
	    },
	    function(){//h:120-180
	    	return [
	    	        mn,
	    	        mx,
	    	        ((arr[0] - 120) / 60) * (mx - mn) + mn
	    	        ];
	    },
	    function(){//h:180-240
	    	return[
	    	       mn,
	    	       ((240-arr[0]) / 60) * (mx - mn) + mn,
	    	       mx
	    	       ];
	    },
	    function(){//h:240-300
	    	return[
	    	       ((arr[0] - 240) / 60) * (mx - mn) + mn,
	    	       mn,
	    	       mx
	    	       ];
	    },
	    function(){//h:300-360
	    	return[
	    	       mx,
	    	       mn,
	    	       ((360 - arr[0]) / 60) * (mx - mn) + mn
	    	       ];
	    }
	];
	return funcs[Math.floor((h % 360) / 60)]();
}
window["arr_rgb_hsv"] = _arr_rgb_hsv;
window["arr_hsv_rgb"] = _arr_hsv_rgb;
})();