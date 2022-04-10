


def shaderToy2Snapchat(data):
    """
    Convert a string that is 
    a valid ShaderToy shader
    into  a shader that is
    consumable by our Custom
    Snapchat SDK App
    """
    # Split
    data = data.split('\n')
    newdata = []

    for i in range(len(data)):
        if not_a_comment(data[i]) and not_empty(data[i]):
            newdata += [line_processor(data[i])]


    # Last check: headers
    newdata = correct_headers(newdata)
    
    # Re-join
    newdata = '\n'.join(newdata)


    return newdata



def not_empty(data: str) -> bool:
	"""
	returns True if the string is not empty
	"""
	return data != ""

def not_a_comment(data: str) -> bool:
	"""
	returns True if the string is not a C++/GLSL comment
	"""
	return True
# TODO: split in // to cover this case: info=1;// comment
#	if '//' in data and ('/*' not in data and '*/' not in data):
#		return False
#	return True

def correct_headers(data: list ) -> str:
    """
    add the headers and/or the variable definition if its not already there
    """
    
    # Define those required
    required = [
	"precision mediump float",
	"out vec4 outColor",
	"in vec3 frgColor",
	"uniform float iGlobalTime",
	]
    
    # Assume they are all not part of the text
    is_required = [True for i in range(len(required))]
    
    # Check if the text already includes some
    for i in range(len(data)):
        for j in range(len(required)):
            if required[j] in data[i]:
                is_required[j] = False

    # Add those required that are not present
    for i,b in enumerate(is_required):
        if b:
            data = [required[i] + ';'] + data
    
    return data


def line_processor(text: str) -> str:
    """
    Apply the rules of conversion that 
    modifies the text line
    """

    # -------------------------------------------------
    # Rule 0) [headers & vars]
    # 0.1 fragCoord -> gl_FragCoord
    if 'fragCoord.' in text:
        text = text.replace("fragCoord.", "gl_FragCoord.")
    elif 'fragCoord' in text:
        text = text.replace("fragCoord", "gl_FragCoord.xy")
    # 0.2 fragColor -> outColor
    text = text.replace("fragColor", "outColor")
    # 0.3 "void mainImage( out vec4 fragColor, in vec2 fragCoord )"
    #      -> 
    #      "void main()"
    if 'void mainImage' in text:
        if '{' in text:
            text = 'void main() {'
        else:
            text = 'void main()'
    #text = text.replace("void mainImage( out vec4 fragColor, in vec2 fragCoord )",
    #                    "void main()")
    #-------------------------------------
    
    # Rule 1) iResolution to some number, i.e. 540,584
    #(see main app's Render/BaseRenderer.cpp at ::draw())
    W=540
    H=584
    if 'iResolution.' in text:
        text = text.replace("iResolution.", f"vec2({W},{H}).")
    elif 'iResolution' in text:
        text = text.replace("fragCoord", f"vec2({W},{H})")


    # Rule 2) iTime to iGlobalTime
    text = text.replace('iTime', 'iGlobalTime')

    # Rule 3) iTimeDelta to ??

    # Rule 4) iFrame to ??

    # Rule 5) iChannelTime[4] to ??

    # Rule 6) iChannelResolution[4] to ??

    # Rule 7) iMouse disabled
    # ...

    # Rule 8) iChannel0..3 to ??

    # Rule 9) iDate disabled
    # ...
    
    # Rule 10) iSampleRate constant to 44100, just for compatibility
    text = text.replace('iSampleRate','44100')

    return text
