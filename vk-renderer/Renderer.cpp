#include "Renderer.h"

#include <chrono>
#include <stdexcept>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "CommandBuffer.h"
#include "DescriptorSetLayout.h"
#include "DescriptorSets.h"
#include "GraphicsPipeline.h"
#include "Instance.h"
#include "SwapChain.h"
#include "Sync.h"
#include "Surface.h"
#include "UniformBufferObject.h"

Renderer::Renderer(const std::shared_ptr<VkDevice_T>& device,
        const std::shared_ptr<Window>& window,
        const std::shared_ptr<VkSurfaceKHR_T>&surface,
        const std::shared_ptr<Mesh>& mesh,
        const std::shared_ptr<Texture>& texture) {
    devicePtr = device;
    meshPtr = mesh;

    QueueFamilyIndices queueFamilyIndices = Instance::getPhysicalDevice()->getQueueFamilyIndices();
    vkGetDeviceQueue(devicePtr.get(), queueFamilyIndices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(devicePtr.get(), queueFamilyIndices.presentFamily.value(), 0, &presentQueue);
    
    swapChain = std::make_unique<SwapChain>(device, surface, window);
    
    descriptorSetLayout = std::make_unique<DescriptorSetLayout>(device);

    graphicsPipeline = std::make_unique<GraphicsPipeline>(device,swapChain->getRenderPass(), descriptorSetLayout->getHandle());

    commandBuffer = std::make_unique<CommandBuffer>(device, maxFramesInFlight);

    uniformBuffers = std::make_shared<UniformBuffers>(device, maxFramesInFlight);
    descriptorSets = std::make_shared<DescriptorSets>(device, descriptorSetLayout->getHandle(), uniformBuffers->getBuffers(),  texture->getImageView(), texture->getTextureSampler(), maxFramesInFlight);
    
    syncObjects = std::make_unique<Sync>(device, maxFramesInFlight);
}

void Renderer::drawFrame() {
    syncObjects->waitForFrameFence(currentFrame);

    //mb move it to SwapChain class
    uint32_t imageIndex;

    //aquireNextImage function
    VkResult result = vkAcquireNextImageKHR(devicePtr.get(), swapChain->getHandle(), UINT64_MAX, syncObjects->getFrameAvailableSemaphore(currentFrame), VK_NULL_HANDLE, &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        swapChain->recreateSwapChain();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    updateUniformBuffer(swapChain->getAspectRatio());
    
    syncObjects->resetFraneFence(currentFrame);
    
    const auto currentBuffer = commandBuffer->getFrameCommandBuffer(currentFrame);    
    
    recordCommandBuffer(currentBuffer, imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {syncObjects->getFrameAvailableSemaphore(currentFrame)};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &currentBuffer;

    VkSemaphore signalSemaphores[] = {syncObjects->getRenderFinishedSemaphore(currentFrame)};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, syncObjects->getFrameFence(currentFrame)) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapChain->getHandle()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;
    
    result = vkQueuePresentKHR(presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
        framebufferResized = false;
        swapChain->recreateSwapChain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    currentFrame = (currentFrame + 1) % maxFramesInFlight;
}


void Renderer::updateUniformBuffer(float aspectRatio) {
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
    startTime = currentTime;
    
    currentRotation += glm::radians(rotationSpeed) * deltaTime;
    if(currentRotation >= 2 * glm::pi<float>())
    {
        currentRotation = 0.0f;
    }
    
    UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), currentRotation, glm::vec3(0.0f, 0.0f, 1.0f));

    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    ubo.proj = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;

    memcpy(uniformBuffers->getFrameUniformBuffer(currentFrame), &ubo, sizeof(ubo));
}

void Renderer::recordCommandBuffer(VkCommandBuffer currentBuffer, uint32_t imageIndex) {
    vkResetCommandBuffer(currentBuffer, /*VkCommandBufferResetFlagBits*/ 0); 
    
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0; // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional

    if (vkBeginCommandBuffer(currentBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }
    const VkRenderPassBeginInfo renderPassInfo =  swapChain->getRenderPassInfo(imageIndex);
    vkCmdBeginRenderPass(currentBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(currentBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->getGraphicsPipeline());

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(renderPassInfo.renderArea.extent.width);
    viewport.height = static_cast<float>(renderPassInfo.renderArea.extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(currentBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = renderPassInfo.renderArea.extent;
    vkCmdSetScissor(currentBuffer, 0, 1, &scissor);

    //Binding the vertex buffer
    VkBuffer vertexBuffers[] = {meshPtr->getVertexBuffer()};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(currentBuffer, 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(currentBuffer, meshPtr->getIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);

    //ubo bind
    const auto descriptorSet = descriptorSets->getFrameDescriptorSet(currentFrame);
    vkCmdBindDescriptorSets(currentBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->getPipelineLayout(), 0, 1, &descriptorSet, 0, nullptr);

    vkCmdDrawIndexed(currentBuffer, meshPtr->getIndexesSize(), 1, 0, 0, 0);

    vkCmdEndRenderPass(currentBuffer);

    if (vkEndCommandBuffer(currentBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}
