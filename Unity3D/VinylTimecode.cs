using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;

public class VinylTimecode : MonoBehaviour {
	public string TimecoderName = "traktor_a";
	public float CurrentSpeed = 0;

	void OnEnable () {
		int result = timecode_library_init (TimecoderName);
		if (result != 0) {
			Debug.LogError ("Cannot init timecode library: Error code " + result);
		}
	}

	void OnDisable () {
		timecode_library_deinit ();
	}
	
	void Update () {
		CurrentSpeed = timecode_library_get_speed ();
	}

	[DllImport("TimecodeLibrary")]  
	static private extern int timecode_library_init(string timecoder_name);  

	[DllImport("TimecodeLibrary")]  
	static private extern float timecode_library_get_speed();  

	[DllImport("TimecodeLibrary")]  
	static private extern void timecode_library_deinit();  
}
