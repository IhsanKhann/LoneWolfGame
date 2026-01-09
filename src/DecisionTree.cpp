#include "DecisionTree.h"
#include <fstream>
#include <iostream>

// ============================================================
// CHANGES: Updated to use Choice struct with StatEffects as described in Listing 4.1
// Implemented navigateToNode as described in Algorithm 1
// Choices now carry stat effect payloads as described in Ch 2.4
// ============================================================

DecisionTree::DecisionTree() : currentNodeId(1) {}

DecisionTree::~DecisionTree() {
    for (auto& pair : nodes) delete pair.second;
}

// Algorithm 1: NavigateToNode implementation
bool DecisionTree::navigateToNode(int targetID) {
    if (nodes.find(targetID) == nodes.end()) {
        std::cerr << "Error: Node " << targetID << " not found" << std::endl;
        return false;
    }
    
    currentNodeId = targetID;
    // Trigger NodeEnterEvent (simplified - can be expanded)
    std::cout << "Navigated to Node " << targetID << std::endl;
    return true;
}

void DecisionTree::loadNodes() {
    // ============================================================
    // UPDATED: All choices now include stat effects where appropriate
    // ============================================================
    
    // --- Nodes and Choices with StatEffects ---
    createNode(1, "You wake up disoriented in the snow. The storm has passed, but you're alone. Your pack is nowhere to be seen. In the distance, you smell fresh blood. But approaching means leaving where your pack might return.");
    addChoice(1, "Follow the scent of blood", 2, {StatEffect(0, 5, -10, 0, 0, 0, 0)});
    addChoice(1, "Wait and howl for pack", 3, {StatEffect(0, 5, -5, 5, 0, 0, 0)});
    addTrigger(1, 1);

    createNode(2, "You track the blood to a frozen stream. A wounded deer stands on thin ice, weak and vulnerable. The ice looks dangerous, but your hunger drives you forward.");
    addChoice(2, "Cross the ice carefully", 4, {StatEffect(0, 5, -15, 0, -5, 0, 0)});
    addChoice(2, "Find another path around", 5, {StatEffect(0, 10, -10, 0, 0, 0, 0)});
    addTrigger(2, 2);

    createNode(3, "Your howl echoes through the mountains. Silence at first... then answering howls! But they don't sound like your pack. These are unfamiliar, possibly hostile wolves approaching from the north.");
    addChoice(3, "Hide and observe them", 6, {StatEffect(0, 5, -10, 0, -5, 0, 0)});
    addChoice(3, "Approach cautiously", 7, {StatEffect(0, 5, -10, 5, 0, 0, 0)});
    addTrigger(3, 3);

    createNode(4, "You step onto the ice. It creaks ominously. The deer watches you nervously, ready to bolt. You must decide quickly.");
    addChoice(4, "Rush forward to catch deer", 8, {StatEffect(0, 0, -20, 0, 0, 5, 10)});
    addChoice(4, "Retreat to safety", 9, {StatEffect(0, 5, 5, 0, 5, 0, 0)});
    addTrigger(4, 4);

    createNode(5, "You circle around the stream, losing precious time. When you finally reach the deer's position, fresh tracks show it fled. But you discover something else - a cave entrance with the scent of other wolves and... food stores.");
    addChoice(5, "Enter the cave cautiously", 10, {StatEffect(0, 0, -10, 0, 0, 0, 5)});
    addChoice(5, "Continue hunting elsewhere", 11, {StatEffect(0, 10, -15, 0, 0, 0, 0)});
    addTrigger(5, 5);

    createNode(6, "Hidden behind rocks, you watch three thin, desperate-looking wolves approach your position. One catches your scent and stops. The leader, a scarred female with one eye, growls a warning in your direction.");
    addChoice(6, "Challenge the scarred leader", 12, {StatEffect(0, 0, -20, -10, 0, 10, 15)});
    addChoice(6, "Show submission and respect", 13, {StatEffect(0, 0, -5, 10, 5, 0, 10)});
    addTrigger(6, 6);

    createNode(7, "You walk toward them with confidence but not aggression. The pack leader, the scarred female, studies you intensely. 'You smell of the mountain pack. Where are they?' Her voice is harsh but curious.");
    addChoice(7, "Tell truth about the storm", 14, {StatEffect(0, 0, -5, 10, 10, 0, 10)});
    addChoice(7, "Claim you're a lone wolf", 15, {StatEffect(0, 0, -5, -20, -10, 0, 0)});
    addTrigger(7, 7);

    // --- Ending Nodes ---
    createNode(8, "You sprint across the ice just as it begins to crack beneath you! Your claws find purchase on the deer and you make a clean kill. The ice holds just long enough to drag your prize to shore. You feast well, restoring your strength. As you eat, you notice wolf tracks - your pack was here recently!", true);
    setNodeEndingType(8, "Successful Hunter - You survive and find hope");
    addTrigger(8, 8);

    createNode(9, "You back away from the dangerous ice. Survival isn't about taking foolish risks. As you turn, you spot fresh rabbit tracks leading into thick brush. Small prey, but much safer than drowning.");
    addChoice(9, "Hunt the rabbit", 16, {StatEffect(0, 5, -10, 0, 0, 0, 5)});
    addChoice(9, "Search for better shelter", 17, {StatEffect(0, 5, -5, 0, 5, 0, 0)});
    addTrigger(9, 9);

    createNode(10, "The cave is empty but clearly used recently. You find food scraps, medicinal herbs, and most valuable - a map carved into the wall showing safe paths, water sources, and dangerous territories. This knowledge could save your life.");
    addChoice(10, "Memorize the map and take supplies", 18, {StatEffect(10, -10, 0, 0, 10, 0, 20)});
    addChoice(10, "Leave respectfully and continue", 19, {StatEffect(0, 5, -5, 5, 5, 0, 5)});
    addTrigger(10, 10);

    createNode(11, "You track new prey for hours through the wilderness. Your energy drains but determination drives you forward. Finally, you corner a rabbit in thick brush. It's small, but you've proven you can survive alone. Days pass as you perfect your hunting skills in this territory.", true);
    setNodeEndingType(11, "Lone Survivor - You adapt to solitary life");
    addTrigger(11, 11);

    createNode(12, "The scarred wolf accepts your challenge. The fight is brutal and fierce. Though you take serious wounds, you prove your strength and courage. Bloodied but standing, you earn her respect. 'You fight with honor. Join us - we need wolves like you.' You've found a new pack.", true);
    setNodeEndingType(12, "Pack Warrior - Earned through battle");
    addTrigger(12, 12);

    createNode(13, "You lower your body and avert your eyes, showing you're no threat. The scarred leader relaxes slightly. 'Smart. Pride gets wolves killed in winter. We hunt together or starve alone. You're welcome with us.' You've found a new family through wisdom, not violence.", true);
    setNodeEndingType(13, "Pack Member - Joined through respect");
    addTrigger(13, 13);

    createNode(14, "You tell her everything - the storm, being separated, your desperate search. She listens carefully, then nods. 'The storm took many from all packs. Your family headed west three days ago, toward the valley. It's dangerous, but I'll guide you there.' True to her word, she leads you through hidden paths. After days of travel, you hear familiar howls. Your pack! The reunion is joyful. You've found your way home.", true);
    setNodeEndingType(14, "Reunion - Found your pack again");
    addTrigger(14, 14);

    createNode(15, "She doesn't believe you. Wolves don't willingly abandon their packs. 'Liar... or outcast?' Her pack surrounds you, growling. You have no choice but to flee into the wilderness. Alone and now marked as untrustworthy, survival becomes much harder. Winter claims you within days.", true);
    setNodeEndingType(15, "Tragic Death - Died alone in winter");
    addTrigger(15, 15);

    // --- Remaining nodes 16-25 ---
    createNode(16, "You catch the rabbit quickly with practiced skill. As you eat, you hear heavy breathing nearby - a large bear, attracted by the scent of fresh blood. It wants your kill.");
    addChoice(16, "Defend your kill from bear", 20, {StatEffect(0, 0, -30, 0, 0, 5, 5)});
    addChoice(16, "Abandon kill and flee", 21, {StatEffect(0, 10, -10, 0, -5, 0, 0)});
    addTrigger(16, 16);

    createNode(17, "You find a perfect hollow beneath a massive fallen tree. It's dry, protected from wind, and defensible. As night falls, you hear wolves fighting in the distance over territory. The wilderness is full of danger.");
    addChoice(17, "Stay hidden in shelter", 22, {StatEffect(5, 5, 10, 0, 10, 0, 5)});
    addChoice(17, "Investigate the wolf fight", 23, {StatEffect(0, 5, -10, 0, -5, 0, 10)});
    addTrigger(17, 17);

    createNode(18, "You carefully study the map, committing every detail to memory. The marked water sources, safe dens, and danger zones become your roadmap to survival. You take some dried meat and herbs left behind. Over the following weeks, this knowledge helps you thrive. You avoid dangers, find resources, and even start marking your own territory. You've become a true survivor.", true);
    setNodeEndingType(18, "Territory Master - Claimed your domain");
    addTrigger(18, 18);

    createNode(19, "You leave the cave undisturbed, respecting another wolf's territory. The wilderness teaches harsh lessons, but you continue forward with dignity.");
    addChoice(19, "Search for your original pack", 24, {StatEffect(0, 10, -15, 5, 0, 0, 5)});
    addChoice(19, "Seek out other lone wolves", 25, {StatEffect(0, 5, -10, 0, 0, 0, 10)});
    addTrigger(19, 19);

    createNode(20, "You bare your fangs and stand your ground against the massive bear. It's a fatal mistake. The bear is far too powerful. Your bravery costs you everything. The wilderness is unforgiving to those who don't know their limits.", true);
    setNodeEndingType(20, "Tragic Death - Killed by bear");
    addTrigger(20, 20);

    createNode(21, "You abandon your kill and flee. Better to lose a meal than your life. The bear takes your rabbit, but you escape unharmed. Hungry but alive, you continue your journey.");
    addChoice(21, "Hunt again immediately", 11, {StatEffect(0, 5, -10, 0, 0, 0, 0)});
    addChoice(21, "Find safe place to rest", 22, {StatEffect(5, 5, 5, 0, 5, 0, 0)});
    addTrigger(21, 21);

    createNode(22, "You stay hidden in your shelter as the sounds of conflict fade into the night. Days pass. You hunt cautiously, rest well, and slowly build strength. You've learned that survival sometimes means avoiding conflict entirely. You become a ghost in the wilderness - unseen, but thriving.", true);
    setNodeEndingType(22, "Shadow Wolf - Survived through caution");
    addTrigger(22, 22);

    createNode(23, "You carefully approach the sounds of conflict. Two wolves are fighting viciously over territory while a third watches. As the fight ends, the loser limps away, badly wounded. The victor doesn't pursue. You could help the wounded wolf, or approach the victor.");
    addChoice(23, "Help the wounded wolf", 24, {StatEffect(0, 5, -10, 10, 10, 0, 15)});
    addChoice(23, "Approach the victor", 25, {StatEffect(0, 5, -10, 5, 0, 5, 10)});
    addTrigger(23, 23);

    createNode(24, "You approach the wounded wolf carefully. At first fearful, they calm as you share the herbs from the cave. As they heal over days, a bond forms. They tell you of other scattered wolves, survivors like you both. Together, you begin forming a new pack - one built on compassion and cooperation rather than dominance. You become the alpha not through strength, but through wisdom and kindness.", true);
    setNodeEndingType(24, "Alpha of the Outcasts - Built new pack");
    addTrigger(24, 24);

    createNode(25, "You approach the victorious wolf with respect. They size you up, impressed by your courage in approaching. 'You have guts. I respect that. These lands are harsh, but together we're stronger.' You join forces, combining hunting grounds and watching each other's backs. It's not a traditional pack, but it's a partnership that works. You've found your place.", true);
    setNodeEndingType(25, "Allied Hunters - Partnership for survival");
    addTrigger(25, 25);
}

// --- Remaining DecisionTree member functions ---

const Node* DecisionTree::getCurrentNode() const {
    auto it = nodes.find(currentNodeId);
    return it != nodes.end() ? it->second : nullptr;
}

bool DecisionTree::makeChoice(int choiceIndex) {
    const Node* current = getCurrentNode();
    if (!current) return false;
    
    const auto& choices = current->getChoicesWithEffects();
    if (choiceIndex < 0 || choiceIndex >= static_cast<int>(choices.size())) return false;
    
    currentNodeId = choices[choiceIndex].targetNodeId;
    return true;
}

void DecisionTree::reset() { 
    currentNodeId = 1; 
}

void DecisionTree::setCurrentNode(int nodeId) { 
    currentNodeId = nodeId; 
}

void DecisionTree::createNode(int id, const std::string& text, bool isEnding) { 
    nodes[id] = new Node(id, text, isEnding); 
}

void DecisionTree::addChoice(int nodeId, const std::string& text, int nextNodeId, const std::vector<StatEffect>& effects) {
    auto it = nodes.find(nodeId);
    if (it != nodes.end()) it->second->addChoice(text, nextNodeId, effects);
}

void DecisionTree::addTrigger(int nodeId, int eventId) {
    auto it = nodes.find(nodeId);
    if (it != nodes.end()) it->second->addTrigger(eventId);
}

void DecisionTree::setNodeEndingType(int nodeId, const std::string& type) {
    auto it = nodes.find(nodeId);
    if (it != nodes.end()) it->second->setEndingType(type);
}

void DecisionTree::generateDotFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) return;

    file << "digraph DecisionTree {\n  rankdir=TB;\n  node [shape=box, style=rounded];\n\n";

    for (const auto& pair : nodes) {
        const Node* node = pair.second;
        std::string shape = node->isEndingNode() ? "doubleoctagon" : "box";
        std::string color = node->isEndingNode() ? ", color=red, style=filled, fillcolor=lightpink" : "";
        file << "  node" << node->getId() << " [label=\"Node " << node->getId() << "\", shape=" << shape << color << "];\n";
    }

    file << "\n";

    for (const auto& pair : nodes) {
        const Node* node = pair.second;
        auto choices = node->getChoices();
        for (size_t i = 0; i < choices.size(); ++i) {
            file << "  node" << node->getId() << " -> node" << choices[i].second << " [label=\"" << static_cast<char>('A' + i) << "\"];\n";
        }
    }

    file << "}\n";
    file.close();
}