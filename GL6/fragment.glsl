#version 330 core

in vec3 FragPos; // 정점 위치 (월드 공간)
in vec3 Normal;  // 변환된 법선 벡터
in vec3 out_Color; // 색상

out vec4 FragColor; // 출력 색상

uniform vec3 lightPos;   // 조명 위치
uniform vec3 lightColor; // 조명 색상
uniform vec3 viewPos;    // 카메라 위치

void main() {
    // Ambient lighting
    float ambientLight = 0.3; // 주변 조명 강도
    vec3 ambient = ambientLight * lightColor; // 주변 조명

    // Diffuse lighting
    vec3 norm = normalize(Normal); // 정규화된 법선 벡터
    vec3 lightDir = normalize(lightPos - FragPos); // 조명 방향 벡터
    float diff = max(dot(norm, lightDir), 0.0); // N·L 계산 (산란 반사 강도)
    vec3 diffuse = diff * lightColor; // 산란 반사 조명

    // Specular lighting
    int shininess = 64; // 광택 계수
    vec3 viewDir = normalize(viewPos - FragPos); // 관찰자 방향 벡터
    vec3 reflectDir = reflect(-lightDir, norm); // 반사 벡터
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess); // 하이라이트 강도
    vec3 specular = spec * lightColor; // 반사 조명

    // Combine all lighting components
    vec3 result = (ambient + diffuse + specular) * out_Color; // 조명과 색상 결합
    FragColor = vec4(result, 1.0); // 최종 색상 출력
}
