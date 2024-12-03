#version 330 core

in vec3 FragPos; // ���� ��ġ (���� ����)
in vec3 Normal;  // ��ȯ�� ���� ����
in vec3 out_Color; // ����

out vec4 FragColor; // ��� ����

uniform vec3 lightPos;   // ���� ��ġ
uniform vec3 lightColor; // ���� ����
uniform vec3 viewPos;    // ī�޶� ��ġ

void main() {
    // Ambient lighting
    float ambientLight = 0.3; // �ֺ� ���� ����
    vec3 ambient = ambientLight * lightColor; // �ֺ� ����

    // Diffuse lighting
    vec3 norm = normalize(Normal); // ����ȭ�� ���� ����
    vec3 lightDir = normalize(lightPos - FragPos); // ���� ���� ����
    float diff = max(dot(norm, lightDir), 0.0); // N��L ��� (��� �ݻ� ����)
    vec3 diffuse = diff * lightColor; // ��� �ݻ� ����

    // Specular lighting
    int shininess = 64; // ���� ���
    vec3 viewDir = normalize(viewPos - FragPos); // ������ ���� ����
    vec3 reflectDir = reflect(-lightDir, norm); // �ݻ� ����
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess); // ���̶���Ʈ ����
    vec3 specular = spec * lightColor; // �ݻ� ����

    // Combine all lighting components
    vec3 result = (ambient + diffuse + specular) * out_Color; // ����� ���� ����
    FragColor = vec4(result, 1.0); // ���� ���� ���
}
