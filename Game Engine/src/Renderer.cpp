#include "GameConstants.h"

void Renderer::renderGameObject(GameObject * pObject, GameObject *mainCamera, GameObject *mainLight)
{
	if (!pObject)
		return;

	pObject->render();

	Mesh * currentMesh = pObject->getMesh();
	Transform * currentTransform = pObject->getTransform();
	//we know is going to be a standard material
	Material * currentMaterial = (Material*)pObject->getMaterial();

	if (currentMesh && currentMaterial && currentTransform)
	{
		currentMaterial->bind();
		currentMesh->bind();

		GLint MVPLocation = currentMaterial->getUniformLocation("MVP");
		GLint ModelLocation = currentMaterial->getUniformLocation("Model");
		GLint ambientMatLocation = currentMaterial->getUniformLocation("ambientMaterialColour");
		GLint ambientLightLocation = currentMaterial->getUniformLocation("ambientLightColour");
		GLint diffuseMatLocation = currentMaterial->getUniformLocation("diffuseMaterialColour");
		GLint diffuseLightLocation = currentMaterial->getUniformLocation("diffuseLightColour");
		GLint lightDirectionLocation = currentMaterial->getUniformLocation("lightDirection");
		GLint specularMatLocation = currentMaterial->getUniformLocation("specularMaterialColour");
		GLint specularLightLocation = currentMaterial->getUniformLocation("specularLightColour");
		GLint specularpowerLocation = currentMaterial->getUniformLocation("specularPower");
		GLint cameraPositionLocation = currentMaterial->getUniformLocation("cameraPosition");
		GLint diffuseTextureLocation = currentMaterial->getUniformLocation("diffuseMap");
		GLint specTextureLocation = currentMaterial->getUniformLocation("specMap");
		GLint bumpTextureLocation = currentMaterial->getUniformLocation("bumpMap");
		GLint heightTextureLocation = currentMaterial->getUniformLocation("heightMap");
		Camera * cam = mainCamera->getCamera();
		Light* light = mainLight->getLight();


		mat4 MVP = cam->getProjection()*cam->getView()*currentTransform->getModel();
		mat4 Model = currentTransform->getModel();

		vec4 ambientMaterialColour = currentMaterial->getAmbientColour();
		vec4 diffuseMaterialColour = currentMaterial->getDiffuseColour();
		vec4 specularMaterialColour = currentMaterial->getSpecularColour();
		float specularPower = currentMaterial->getSpecularPower();

		vec4 diffuseLightColour = light->getDiffuseColour();
		vec4 specularLightColour = light->getSpecularColour();
		vec3 lightDirection = light->getDirection();

		vec3 cameraPosition = mainCamera->getTransform()->getPosition();

		glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, glm::value_ptr(Model));
		glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(MVP));
		glUniform4fv(ambientMatLocation, 1, glm::value_ptr(ambientMaterialColour));
		glUniform4fv(ambientLightLocation, 1, glm::value_ptr(vec4(1.0f, 1.0f, 1.0f, 1.0f)));

		glUniform4fv(diffuseMatLocation, 1, glm::value_ptr(diffuseMaterialColour));
		glUniform4fv(diffuseLightLocation, 1, glm::value_ptr(diffuseLightColour));
		glUniform3fv(lightDirectionLocation, 1, glm::value_ptr(lightDirection));

		glUniform4fv(specularMatLocation, 1, glm::value_ptr(specularMaterialColour));
		glUniform4fv(specularLightLocation, 1, glm::value_ptr(specularLightColour));

		glUniform3fv(cameraPositionLocation, 1, glm::value_ptr(cameraPosition));
		glUniform1f(specularpowerLocation, specularPower);

		glUniform1i(diffuseTextureLocation, 0);
		glUniform1i(specTextureLocation, 1);
		glUniform1i(bumpTextureLocation, 2);
		glUniform1i(heightTextureLocation, 3);

		glDrawElements(GL_TRIANGLES, currentMesh->getIndexCount(), GL_UNSIGNED_INT, 0);

		currentMaterial->unbind();
	}

	for (int i = 0; i < pObject->getChildCount(); i++)
	{
		renderGameObject(pObject->getChild(i), mainCamera, mainLight);
	}
}

void Renderer::renderSphere()
{
	/*glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	glTranslatef(m_Position.x, m_Position.y, m_Position.z);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Rotate Sphere to take account of gluSphere caps
	glRotatef(getRotAngle(), 0, 0, 1);	// Rotation is now on z rather than Y to make sphere spin

	gluSphere(quad, sRadius, sSlices, sStacks);

	glDisable(GL_NORMALIZE);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();*/
}

void Renderer::renderSkyBox(OpenGLLoader *openGLLoader, GameObject *goSkyBox, GameObject *goMainCamera)
{
	goSkyBox->render();

	Mesh * currentMesh = goSkyBox->getMesh();
	SkyBoxMaterial * currentMaterial = (SkyBoxMaterial*)goSkyBox->getMaterial();
	if (currentMesh && currentMaterial)
	{
	Camera * cam = goMainCamera->getCamera();

	currentMaterial->bind();
	currentMesh->bind();

	GLint cameraLocation = currentMaterial->getUniformLocation("cameraPos");
	GLint viewLocation = currentMaterial->getUniformLocation("view");
	GLint projectionLocation = currentMaterial->getUniformLocation("projection");
	GLint cubeTextureLocation = currentMaterial->getUniformLocation("cubeTexture");

	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(cam->getProjection()));
	mat4 rotationY = glm::rotate(mat4(1.0f), goMainCamera->getTransform()->getRotation().y, vec3(0.0f, 1.0f, 0.0f));
	mat4 rotationX = glm::rotate(mat4(1.0f), goMainCamera->getTransform()->getRotation().x, vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(rotationY*rotationX));
	glUniform4fv(cameraLocation, 1, glm::value_ptr(goMainCamera->getTransform()->getPosition()));
	glUniform1i(cubeTextureLocation, 0);

	glDrawElements(GL_TRIANGLES, currentMesh->getIndexCount(), GL_UNSIGNED_INT, 0);

	currentMaterial->unbind();
	}

	openGLLoader->checkForErrors();
}

//Function to render(aka draw)
void Renderer::render(OpenGLLoader *openGLLoader, std::vector<GameObject*> gameObjects, PostProcessing postProcessor, GameObject *goMainCamera, GameObject *goMainLight)
{
	//Bind Framebuffer
	//postProcessor.bind();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	//clear the colour and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//renderSkyBox();

	//alternative sytanx
	for (auto iter = gameObjects.begin(); iter != gameObjects.end(); iter++)
	{
		renderGameObject((*iter), goMainCamera, goMainLight);
	}

	//now switch to normal framebuffer
	//postProcessor.preDraw();
	//Grab stuff from shader
	GLint colourFilterLocation = postProcessor.getUniformVariableLocation("colourFilter");
	glUniformMatrix3fv(colourFilterLocation, 1, GL_FALSE, glm::value_ptr(SEPIA_FILTER));

	//draw
	//postProcessor.draw();

	//post draw
	//postProcessor.postDraw();

	SDL_GL_SwapWindow(openGLLoader->getWindow());
}