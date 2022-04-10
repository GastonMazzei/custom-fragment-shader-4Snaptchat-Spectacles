import re

# Define texture parsers globally
textcall = re.compile(r'(texture\([a-zA-Z0-9\/\. \*\+\-]+[ ]*,[ ]*[a-zA-Z0-9\/\. \*\+\-]+[ ]*\))')
nelems = re.compile(r'texture\([a-zA-Z0-9\/\. \*\+\-]+[ ]*,[ ]*[a-zA-Z0-9\/\. \*\+\-]+[ ]*\)[\.]*([rgb]+)')


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

    # Correct the names in case ShaderToy users changed "fragCoord' to some other thing
    found = False
    for line in data:
        if 'mainImage' in line:
            pattern = re.compile(r'mainImage[ ]*\([ ]*out[ ]*vec4[ ]*([a-zA-Z0-9]+)[ ]*,[ ]*in[ ]*vec2[ ]*([a-zA-Z0-9]+)')
            found = re.findall(pattern,line)
            if len(found) > 0:
                color,coord = found[0]
                found = True
                print(f'detected names are: {color} and {coord}')
            break
    if found:
        for i in range(len(data)):
            if color in data[i]:
                data[i] = data[i].replace(color, 'fragColor')
            if coord in data[i]:
                data[i] = data[i].replace(coord, 'fragCoord')

    # Transform the lines accordingly
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


def fixTexture(TEXT):
	global textcall
	global nelems
	texttoreplace = re.findall(textcall, TEXT)
	if len(texttoreplace) > 0:
		texttoreplace = texttoreplace[0]
		num = re.findall(nelems, TEXT)
		num = len(num[0]) if len(num)>0 else 1
		if num==1:
			TEXT = TEXT.replace(texttoreplace, f"1.0")
		elif num==2:	
			TEXT = TEXT.replace(texttoreplace, f"vec2(1.,1.)")
		elif num==3:
			TEXT = TEXT.replace(texttoreplace, f"vec3(1.,1.,1.)")
		elif num==4:
			TEXT = TEXT.replace(texttoreplace, f"vec4(1.,1.,1.)")
	return TEXT


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
    W=540 * 2
    H=584
    text = text.replace("iResolution", f"vec3({W},{H},1000)")

    # Rule 2) iTime to iGlobalTime
    text = text.replace('iTime', 'iGlobalTime')

    # Rule 3) iTimeDelta to iGlobalTime
    text = text.replace('iTimeDelta', 'iGlobalTime')

    # Rule 4) iFrame to 0
    text = text.replace('iFrame',"0")

    # Rule 5) iChannelTime[4] to vec4(iGlobalTime,iGlobalTime,iGlobalTime,iGlobalTime)
    text = text.replace('iChannelTime','vec4(iGlobalTime,iGlobalTime,iGlobalTime,iGlobalTime)')

    # Rule 6) iChannelResolution[4] to smth
    text = text.replace("iChannelResolution[0]", f"vec3({W},{H},1000)")
    text = text.replace("iChannelResolution[1]", f"vec3({W},{H},1000)")
    text = text.replace("iChannelResolution[2]", f"vec3({W},{H},1000)")
    text = text.replace("iChannelResolution[3]", f"vec3({W},{H},1000)")

    # Rule 7) iMouse disabled, hardcoded to 100,100,100,100
    text = text.replace('iMouse','vec4(100,100,100,100)')

    # Rule 8) iChannel0..3 to ??
    text = fixTexture(text)

    # Rule 9) iDate disabled
    text = text.replace('iDate','vec4(2022,04,09,iGlobalTime)')
    
    # Rule 10) iSampleRate constant to 44100, just for compatibility
    text = text.replace('iSampleRate','44100')

    return text
